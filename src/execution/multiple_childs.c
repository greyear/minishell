#include "../../include/minishell.h"

/**
 * @brief Waits for all child processes to finish and updates the shell's exit status.
 * 
 * This function waits for each child process created by a command to terminate. It 
 * specifically tracks the exit status of the last executed command and updates the 
 * shell's exit status accordingly. If the last command terminates normally, its exit 
 * code is stored in `ms->exit_status`. If it is terminated by a signal, the exit status 
 * is set to 130 for SIGINT or 131 for SIGQUIT.
 * 
 * @param num_cmds The number of child processes to wait for.
 * @param last_pid The process ID of the last executed command, used to determine 
 *                 the shell's final exit status.
 * @param ms A pointer to the `t_ms` structure, which stores the shell's exit status.
 * 
 * @return None. Modifies `ms->exit_status` based on the termination status of the last process.
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
 * This function sets up pipes and redirects input/output for the child process. 
 * If the command is a built-in, it is executed directly, and the child process 
 * exits with the appropriate status. Otherwise, the function switches the signal 
 * mode and executes an external command using `execve()`. All file descriptors 
 * are closed before execution to prevent leaks.
 * 
 * @param cur A pointer to the `t_cmd` structure containing command details such as 
 *            arguments and input/output files.
 * @param p A pointer to the `t_pipe` structure, which manages pipes, file descriptors, 
 *          and process execution context.
 * 
 * @return None. The function either executes the command or exits the child process.
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
 * @brief Initializes the `t_pipe` structure for managing command execution.
 * 
 * This function sets up the `t_pipe` structure, which handles multiple commands in 
 * a pipeline. It initializes values such as the number of commands, process tracking 
 * variables, and allocates memory for storing process IDs. If memory allocation fails, 
 * an error is printed.
 * 
 * @param p A pointer to the `t_pipe` structure to be initialized.
 * @param num_cmds The total number of commands in the pipeline.
 * @param ms A pointer to the `t_ms` structure, which contains shell-related data.
 * 
 * @return None. The function modifies `p` and prints an error message if memory allocation fails.
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