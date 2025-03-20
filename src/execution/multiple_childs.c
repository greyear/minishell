#include "../../include/minishell.h"

/**
 * @brief Waits for the completion of child processes and updates the exit 
 *        status.
 * 
 * This function waits for all child processes to terminate, checks their exit 
 * status, and updates the shell's exit status accordingly. If a child process 
 * is terminated by a signal, the function handles specific signals (SIGINT and 
 * SIGQUIT) and updates the exit status. Additionally, it manages closing file 
 * descriptors and freeing resources related to the child processes.
 * 
 * - If a child process exits normally, the exit status is captured.
 * - If the child process is terminated by a signal, the exit status is updated 
 *   to reflect the signal that caused termination (SIGINT or SIGQUIT).
 * - The function ensures that the correct exit status is propagated, with 
 *   special handling for interactive signals.
 * 
 * @param num_cmds The number of child commands (processes) to wait for.
 * @param last_pid The PID of the last child process.
 * @param ms The main shell structure, used to update the exit status.
 * @param p A structure containing pipe resources, used to manage 
 *          file descriptors.
 */

static void	wait_for_children(int num_cmds, pid_t last_pid, t_ms *ms, t_pipe *p)
{
	int		i;
	int		status;
	pid_t	wpid;

	i = 0;
	close_all_fds(p);
	while (i < num_cmds)
	{
		wpid = wait(&status);
		if (wpid == last_pid && WIFEXITED(status))
			ms->exit_status = WEXITSTATUS(status);
		if (WIFSIGNALED(status))
		{
			if (wpid == last_pid && WTERMSIG(status) == SIGINT)
				ms->exit_status = 130;
			if (wpid == last_pid && WTERMSIG(status) == SIGQUIT)
				ms->exit_status = 131;
			if (WTERMSIG(status) == SIGINT)
				write(STDERR_FILENO, "\n", 1);
		}
		i++;
	}
	if (ms->exit_status == 131)
		write(STDERR_FILENO, "Quit\n", 5);
	free_pids(p);
}

/**
 * @brief Executes a command in a child process.
 * 
 * This function sets up pipes and redirects input/output for the child 
 * process. If the command is a built-in, it is executed directly, and the 
 * child process exits with the appropriate status. Otherwise, the function 
 * switches the signal mode and executes an external command using `execve()`. 
 * All file descriptors are closed before execution to prevent leaks.
 * 
 * @param cur A pointer to the `t_cmd` structure containing command details 
 *            such as arguments and input/output files.
 * @param p A pointer to the `t_pipe` structure, which manages pipes, file 
 *          descriptors, and process execution context.
 * 
 * @return None. The function either executes the command or exits the 
 *         child process.
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
 * - In the parent process, it closes unused file descriptors and updates 
 *   pipe tracking variables.
 * 
 * @param cur A pointer to the `t_cmd` structure containing the command's 
 *            arguments, input, and output files.
 * @param p A pointer to the `t_pipe` structure containing pipe-related 
 *          information.
 */
static void	fork_and_execute(t_cmd *cur, t_pipe *p)
{
	if (pipe(p->fd) == -1)
	{
		perror("pipe failed");
		p->ms->exit_status = SYSTEM_ERR;
		return ;
	}
	p->pids[p->cmd_num] = fork();
	if (p->pids[p->cmd_num] < 0)
	{
		perror("fork failed");
		p->ms->exit_status = SYSTEM_ERR;
		return ;
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
 * This function sets up the `t_pipe` structure, which handles multiple commands 
 * in a pipeline. It initializes values such as the number of commands, process 
 * tracking variables, and allocates memory for storing process IDs. If memory 
 * allocation fails, an error is printed.
 * 
 * @param p A pointer to the `t_pipe` structure to be initialized.
 * @param num_cmds The total number of commands in the pipeline.
 * @param ms A pointer to the `t_ms` structure, which contains shell-related
 *           data.
 * 
 * @return None. The function modifies `p` and prints an error message if memory 
 *         allocation fails.
 */
static void	initialize_p(t_pipe *p, int num_cmds, t_ms *ms)
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
 * @brief Creates and manages multiple child processes to execute commands in 
 *        a pipeline.
 * 
 * This function forks a child process for each command in a pipeline, executes 
 * the commands in parallel, and waits for their completion. It handles the 
 * initialization of pipes, forks the processes, and manages the process
 * execution flow. After all child processes have been created and executed,
 * the function waits for their termination and updates the shell's 
 * exit status accordingly.
 * 
 * - Each child process is created using `fork_and_execute`, and pipes are
 *   set up to connect the commands in the pipeline.
 * - The function ensures that empty commands are skipped and that resources are 
 *   freed in case of errors such as memory allocation failures or system errors.
 * 
 * @param num_cmds The number of child processes (commands) to create 
 *                 and execute.
 * @param cmds A linked list of command structures, each containing the
 *             arguments and details of the command to be executed.
 * @param ms The main shell structure, used to track the exit status and manage 
 *           shell state.
 */
void	make_multiple_childs(int num_cmds, t_cmd *cmds, t_ms *ms)
{
	t_pipe	p;
	t_cmd	*cur;

	cur = cmds;
	initialize_p(&p, num_cmds, ms);
	if (!p.pids)
		return ;
	while (p.cmd_num < p.num_cmds && cur)
	{
		if (!cur->args || !cur->args[0])
		{
			p.ms->exit_status = 0;
			cur = cur->next;
			p.cmd_num++;
			continue ;
		}
		fork_and_execute(cur, &p);
		if (ms->exit_status == MALLOC_ERR
			|| ms->exit_status == SYSTEM_ERR)
			return ;
		cur = cur->next;
		p.cmd_num++;
	}
	wait_for_children(p.num_cmds, p.last_pid, p.ms, &p);
}
