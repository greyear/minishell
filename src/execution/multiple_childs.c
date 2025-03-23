#include "../../include/minishell.h"

/**
 * @brief Waits for all child processes to finish and updates the exit status.
 *
 * This function waits for all child processes to terminate, checks their exit 
 * statuses, and updates the minishell's exit status based on the results. If 
 * a child process terminates with a signal, it adjusts the exit status 
 * accordingly. Specifically, if the process was terminated by `SIGINT`, it 
 * sets the exit status to 130, and if by `SIGQUIT`, it sets it to 131. 
 * Additionally, it writes the appropriate message to `stderr` when a process 
 * exits due to a signal.
 *
 * @param last_pid The PID of the last child process, used to check if it was 
 * the one that finished.
 * @param ms The minishell structure containing the exit status and other 
 * relevant state.
 * @param p The current pipe structure containing the number of commands and 
 * other pipe-related info.
 */
static void	wait_for_children(pid_t last_pid, t_ms *ms, t_pipe *p)
{
	int		i;
	int		status;
	pid_t	wpid;

	i = 0;
	while (i < p->num_cmds)
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
}

/**
 * @brief Handles the execution of a child process in a pipeline.
 *
 * This function manages the execution of a command in a child process. It 
 * checks if the command has arguments and if not, it closes file descriptors 
 * and exits. It sets up pipes for communication between processes, redirects 
 * input/output as necessary, and handles any builtins or external commands. 
 * The function ensures proper cleanup of file descriptors, handles error 
 * conditions, and manages signals for external commands.
 *
 * @param cur The current command structure containing the command arguments, 
 * input/output files, etc.
 * @param p The pipeline structure containing information about the current 
 * pipeline state.
 */
static void	child_process(t_cmd *cur, t_pipe *p)
{
	if (!cur->args || !cur->args[0])
	{
		close_fds2(cur->infile, cur->outfile);
		exit(0);
	}
	setup_pipes(p->fd, p->cmd_num, p->num_cmds, p->cur_fd);
	redirect_process(cur->infile, cur->outfile, p->ms);
	close_fds2(cur->infile, cur->outfile);
	close_all_fds(p);
	if (p->ms->exit_status == SYSTEM_ERR)
		exit(SYSTEM_ERR);
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
	close_fds2(cur->infile, cur->outfile);
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
 * @brief Creates multiple child processes to execute commands in parallel.
 *
 * This function handles the creation of child processes for each command in a 
 * pipeline, by forking a new process for each one and executing the corresponding 
 * command. It initializes the necessary pipes, handles the process forking, and 
 * ensures that the file descriptors are properly managed. After forking the child 
 * processes, it waits for them to complete and updates the minishell's exit 
 * status accordingly.
 *
 * @param num_cmds The number of commands to execute in the pipeline.
 * @param cmds A linked list of command structures, each representing a command 
 * to be executed.
 * @param ms The minishell structure containing the environment and exit status.
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
		fork_and_execute(cur, &p);
		close_fds2(cur->infile, cur->outfile);
		if (ms->exit_status == MALLOC_ERR
			|| ms->exit_status == SYSTEM_ERR)
			return ;
		cur = cur->next;
		p.cmd_num++;
	}
	wait_for_children(p.last_pid, p.ms, &p);
	close_fds(cmds);
	close_all_fds(&p);
	free_pids(&p);
}
