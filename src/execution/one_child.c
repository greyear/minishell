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
 * @brief Creates a child process to execute a single command.
 * 
 * This function forks a new process to execute a given command. If the fork fails, 
 * an error message is printed, and the shell's exit status is set to 1. The child 
 * process executes the command using `execute_child()`, while the parent process 
 * waits for the child to finish and updates the shell's exit status based on the 
 * child's termination status.
 * 
 * @param cmd A pointer to the `t_cmd` structure containing the command and its arguments.
 * @param ms A pointer to the `t_ms` structure, which manages shell-related data such 
 *           as environment variables and exit status.
 * 
 * @return None. The function modifies `ms->exit_status` based on the child's termination.
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
		ms->exit_status = 1;
		return;
	}
	if (pid == 0)
		execute_child(cmd, ms);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		ms->exit_status = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			ms->exit_status = 130;
		if (WTERMSIG(status) == SIGQUIT)
			ms->exit_status = 131;
	}
}