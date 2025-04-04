/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   one_child.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 13:52:38 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 13:52:40 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Executes a child process for a command.
 * 
 * Handles redirections, closes unnecessary file descriptors, and checks  
 * for system errors. If the command is a builtin, it executes and exits  
 * with the appropriate status. Otherwise, it proceeds to execute it  
 * as an external command.  
 * 
 * @param cmd The command structure containing arguments and file descriptors.  
 * @param ms The minishell struct for cleanup and error handling.  
 * 
 * @return This function does not return; it either executes or exits with:  
 *         - `SYSTEM_ERR` if a system-related error occurs.  
 *         - The exit status of the executed builtin.  
 *         - The exit status of the external command execution.  
 */
void	execute_child(t_cmd *cmd, t_ms *ms)
{
	int		exit_num;

	redirect_process(cmd->infile, cmd->outfile, ms);
	close_every_cmds_fds(cmd);
	if (ms->exit_status == SYSTEM_ERR)
	{
		clean_in_child(ms);
		exit(SYSTEM_ERR);
	}
	signal_mode(DEFAULT);
	if (is_builtin(cmd))
	{
		handle_builtin(cmd, ms, 1);
		exit_num = ms->exit_status;
		clean_in_child(ms);
		exit(exit_num);
	}
	else
		execute_command(ms->envp, cmd->args, ms);
}

/**
 * @brief Creates and executes a single child process for a command.
 * 
 * This function forks a child process to execute a single command. After 
 * forking, the parent process waits for the child process to complete. The 
 * exit status of the child process is checked to update the shell's exit 
 * status based on its termination (normal exit or signal termination).
 * 
 * - If the command arguments are empty, the function returns without forking.
 * - If the fork fails, an error message is printed, and the shell exit 
 *   status is set to `SYSTEM_ERR`.
 * - If the child process terminates normally, the shell's exit status is 
 *   updated accordingly.
 * - If the child process is terminated by a signal (SIGINT or SIGQUIT), 
 *   the exit status is updated to reflect the corresponding signal.
 * 
 * @param cmd The command structure containing the arguments and details of 
 *            the command to be executed.
 * @param ms The main shell structure, used to track the exit status and 
 *           manage shell state.
 */
void	make_one_child(t_cmd *cmd, t_ms *ms)
{
	pid_t	pid;
	int		status;

	if (!cmd->args || !cmd->args[0])
		return ;
	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		ms->exit_status = SYSTEM_ERR;
		return ;
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
