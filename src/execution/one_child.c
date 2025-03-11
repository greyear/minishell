#include "../../include/minishell.h"

/**
 * @brief Executes a command in a child process, handling redirection and built-in commands.
 * 
 * This function redirects input and output files for the current command using `redirect_process`. Then, it checks
 * if the command is a built-in. If it is, it handles the built-in command with `handle_builtin` and exits the child
 * process with the corresponding exit status. If the command is not a built-in, it is executed using `execute_command`.
 * 
 * @param cmd A pointer to the `t_cmd` structure containing the command details, including the command arguments,
 *            input file, and output file for redirection.
 * @param ms A pointer to the `t_ms` structure that holds the global execution state, such as exit status.
 * 
 * @return This function does not return; it either executes the command or exits the child process after handling
 *         the built-in or executing the command.
 */

void	execute_child(t_cmd *cmd, t_ms *ms)
{
	redirect_process(cmd->infile, cmd->outfile);
    close(ms->saved_stdin);
    close(ms->saved_stdout);
    if (is_builtin(cmd))
    {
        handle_builtin(cmd, ms, 1);
        exit(ms->exit_status);
    }
    else
        execute_command(ms->envp, cmd->args);
}

/**
 * @brief Creates a child process to execute a single command, handling file redirection and heredocs.
 * 
 * This function first checks if valid arguments and file descriptors are provided for the command. It then forks a
 * child process to execute the command using `execute_child`. After the child process completes, the parent process
 * waits for the child to exit using `waitpid`, and updates the global exit status based on the child's exit status.
 * 
 * @param cmd A pointer to the `t_cmd` structure containing the command details, including arguments, input file,
 *            and output file for redirection.
 * @param ms A pointer to the `t_ms` structure that holds the global execution state, including the exit status.
 * 
 * @return This function does not return; it either forks and executes the command in a child process or handles
 *         errors such as invalid file descriptors.
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
        ms->exit_status = 1;
        return;
    }
    if (pid == 0)
        execute_child(cmd, ms);
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
		ms->exit_status = WEXITSTATUS(status);
}