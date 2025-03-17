#include "../../include/minishell.h"

/**
 * @brief Waits for all child processes to finish and updates the exit status of the last child process.
 * 
 * This function waits for a number of child processes to finish based on the `num_cmds` argument. It checks the status
 * of each child process, and if the last child process (identified by `last_pid`) has exited successfully, it updates
 * the `exit_status` in the `ms` structure with the exit status of that process.
 * 
 * @param num_cmds The total number of child processes to wait for.
 * @param last_pid The process ID of the last child process. The exit status of this process is recorded.
 * @param ms A pointer to the `t_ms` structure where the exit status of the last child process is stored.
 * 
 * @return This function does not return; it waits for the specified child processes and updates the `exit_status`.
 */

void	wait_for_children(int num_cmds, pid_t last_pid, t_ms *ms)
{
	int	i;
	int	status;
	pid_t	wpid;

	i = 0;
	while (i < num_cmds)
	{
		wpid = wait(&status);
		if (wpid == last_pid && WIFEXITED(status))
			ms->exit_status = WEXITSTATUS(status);
		if (wpid == last_pid && WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
				ms->exit_status = 130;
			if (WTERMSIG(status) == SIGQUIT)
				ms->exit_status = 131;
		}
		i++;
	}
}

/**
 * @brief Executes a command in a child process.
 * 
 * This function is responsible for setting up pipes, redirecting input/output as needed, 
 * and executing either a built-in command or an external command in a child process.
 * 
 * - First, it configures pipes based on the process's position in the pipeline.
 * - Then, it redirects input and output file descriptors as required.
 * - If the command is a built-in, it executes the built-in function and exits.
 * - Otherwise, it executes an external command using `execve()`.
 * 
 * @param cur A pointer to the `t_cmd` structure containing the command's arguments, input, and output files.
 * @param p A pointer to the `t_pipe` structure containing pipe-related information.
 * 
 * @note This function does not return; it either executes a command or exits the child process.
 */

static void	child_process(t_cmd *cur, t_pipe *p)
{
	setup_pipes(p->fd, p->cmd_num, p->num_cmds, p->cur_fd);
	redirect_process(cur->infile, cur->outfile);
	close_all_fds(p);
	if (is_builtin(cur))
	{
		handle_builtin(cur, p->ms, 1);
		exit(p->ms->exit_status);
	}
	else
	{
		//we are not sure if we need to put it for ALL child processes or only for externals
		signal_mode(DEFAULT);
		execute_command(p->ms->envp, cur->args);
	}
}

/**
 * @brief Creates a child process to execute a command and sets up pipes.
 * 
 * This function performs the following steps:
 * - Creates a new pipe for inter-process communication.
 * - Forks a child process to execute a command.
 * - If the fork fails, it updates the shell's exit status and returns.
 * - In the child process, it calls `child_process()` to execute the command.
 * - In the parent process, it closes unused file descriptors and updates pipe tracking variables.
 * 
 * @param cur A pointer to the `t_cmd` structure containing the command's arguments, input, and output files.
 * @param p A pointer to the `t_pipe` structure containing pipe-related information.
 * 
 * @note If `fork()` fails, the function sets `p->ms->exit_status` to 1 and returns without executing the command.
 */

static void	fork_and_execute(t_cmd *cur, t_pipe *p)
{
	if (pipe(p->fd) == -1)
	{
		perror("pipe failed\n");
		p->ms->exit_status = 1;
		return;
	}
	p->pids[p->cmd_num] = fork(); 
	if (p->pids[p->cmd_num] < 0)
	{
		perror("fork failed\n");
		p->ms->exit_status = 1;
		return;
	}
	if (p->pids[p->cmd_num] == 0)
		child_process(cur, p);
	close_fds2(p->cur_fd, p->fd[1]);
	p->cur_fd = p->fd[0];
	p->last_pid = p->pids[p->cmd_num];
}

/**
 * @brief Initializes the `t_pipe` structure for handling multiple commands in a pipeline.
 * 
 * This function sets up the `t_pipe` structure by:
 * - Storing the total number of commands.
 * - Associating it with the main shell structure (`ms`).
 * - Initializing tracking variables such as `last_pid`, `cmd_num`, and `cur_fd`.
 * - Allocating memory for storing process IDs (`pids`), which will be used to track child processes.
 * 
 * @param p A pointer to the `t_pipe` structure to be initialized.
 * @param num_cmds The total number of commands in the pipeline.
 * @param ms A pointer to the `t_ms` structure that holds shell-related information.
 */

void	initialize_p(t_pipe *p, int num_cmds, t_ms *ms)
{
	p->num_cmds = num_cmds;
	p->ms = ms;
	p->last_pid = -1;
	p->cmd_num = 0;
	p->cur_fd = -1;
	p->pids = (pid_t *)malloc((p->num_cmds) * sizeof(pid_t));
	if (!p->pids)
		print_malloc_error();
}

/**
 * @brief Creates multiple child processes to execute a pipeline of commands.
 * 
 * This function manages the execution of multiple commands in a pipeline by:
 * - Initializing a `t_pipe` structure to track child processes.
 * - Iterating through the list of commands, forking child processes for each.
 * - Handling cases where a command has no arguments.
 * - Closing file descriptors and waiting for all child processes to complete.
 * - Cleaning up allocated resources after execution.
 * 
 * @param num_cmds The total number of commands to execute.
 * @param cmds A linked list of commands to be executed.
 * @param ms A pointer to the `t_ms` structure containing shell-related data.
 */

void	make_multiple_childs(int num_cmds, t_cmd *cmds, t_ms *ms)
{
	t_pipe	p;
	t_cmd	*cur;

	cur = cmds;
	initialize_p(&p, num_cmds, ms);
	if (!p.pids)
		return;
	while (p.cmd_num < p.num_cmds && cur)
	{
		if (!cur->args || !cur->args[0])
		{
			p.ms->exit_status = 0;
			cur = cur->next;
			p.cmd_num++;
			continue;
		}
		fork_and_execute(cur, &p);
		cur = cur->next;
		p.cmd_num++;
	}
	close_all_fds(&p);
	wait_for_children(p.num_cmds, p.last_pid, p.ms);
	free_pids(&p);
}