#include "../../include/minishell.h"

/**
 * @brief Executes a command within a child process.
 * 
 * This function handles input/output redirection and determines whether the command 
 * is a built-in or an external program. If the command is a built-in, it is executed, 
 * and the child process exits with the appropriate status. Otherwise, the function 
 * sets the signal mode and executes an external command using `execve()`.
 * 
 * @param cmd A pointer to the `t_cmd` structure containing command details, such as 
 *            arguments and input/output files.
 * @param ms A pointer to the `t_ms` structure, which holds shell-related data, including 
 *           environment variables and exit status.
 * 
 * @return None. The function either executes the command or exits the child process.
 */

void	execute_child(t_cmd *cmd, t_ms *ms)
{
	redirect_process(cmd->infile, cmd->outfile);
	if (is_builtin(cmd))
	{
		handle_builtin(cmd, ms, 1);
		exit(ms->exit_status);
	}
	else
	{
		//we are not sure if we need to put it for ALL child processes or only for externals
		signal_mode(DEFAULT);
		execute_command(ms->envp, cmd->args);
	}
}

/**
 * @brief Creates and executes a single child process for a command.
 * 
 * This function forks a child process to execute a single command. After forking, 
 * the parent process waits for the child process to complete. The exit status of 
 * the child process is checked to update the shell's exit status based on its 
 * termination (normal exit or signal termination).
 * 
 * - If the command arguments are empty, the function returns without forking.
 * - If the fork fails, an error message is printed, and the shell exit status is 
 *   set to `SYSTEM_ERR`.
 * - If the child process terminates normally, the shell's exit status is updated 
 *   accordingly.
 * - If the child process is terminated by a signal (SIGINT or SIGQUIT), the exit 
 *   status is updated to reflect the corresponding signal.
 * 
 * @param cmd The command structure containing the arguments and details of the command 
 *            to be executed.
 * @param ms The main shell structure, used to track the exit status and manage shell state.
 */

void	make_one_child(t_cmd *cmd, t_ms *ms)
{
	pid_t	pid;
	int		status;

	if (!cmd->args || !cmd->args[0])
		return;
	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		ms->exit_status = SYSTEM_ERR;
		return;
	}
	if (pid == 0)
		execute_child(cmd, ms);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		ms->exit_status = WEXITSTATUS(status);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
			ms->exit_status = 130;
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
			ms->exit_status = 131;
	if (ms->exit_status == 130)
		write(STDERR_FILENO, "\n", 1);
	if (ms->exit_status == 131)
		write(STDERR_FILENO, "Quit\n", 5);
}