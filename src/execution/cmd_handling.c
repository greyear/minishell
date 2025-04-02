/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_handling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 13:52:22 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 13:52:24 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Executes commands given as absolute or relative paths.
 * 
 * Checks if the command is a directory, an existing file, or an executable.  
 * If it is a directory, prints an error and exits. If it exists but lacks  
 * execution permissions, prints an error and exits. If it does not exist,  
 * prints an error and exits. If executable, attempts to run it with execve.  
 * 
 * @param envp The environment variables.  
 * @param cmds The command and its arguments.  
 * @param ms The minishell struct for cleanup.  
 * 
 * @return This function does not return; it either executes or exits with:  
 *         - `126` if the command is a directory or lacks execution perms.  
 *         - `127` if the command does not exist.  
 */
void	handle_absolute_or_relative_path(char **envp, char **cmds, t_ms *ms)
{
	DIR	*dir;

	dir = opendir(cmds[0]);
	if (dir)
	{
		closedir(dir);
		print_cmd_error(cmds[0], IS_DIR);
		clean_in_child(ms);
		exit(CMD_EXEC);
	}
	if (access(cmds[0], F_OK) == 0)
	{
		if (access(cmds[0], X_OK) == 0)
			execve(cmds[0], cmds, envp);
		print_cmd_error(cmds[0], PERM_DEN);
		clean_in_child(ms);
		exit(CMD_EXEC);
	}
	print_cmd_error(cmds[0], NO_FILE_OR_DIR);
	clean_in_child(ms);
	exit(CMD_NF);
}

/**
 * @brief Checks if the command consists only of dots (`.`).
 * 
 * Iterates through the command to verify if it only contains dots. If so,  
 * prints an error, cleans up, and exits.  
 * 
 * @param cmds The command and its arguments.  
 * @param ms The minishell struct for cleanup.  
 * 
 * @return This function does not return; it exits with `127` if the command  
 *         consists only of dots.  
 */
void	check_if_dot(char **cmds, t_ms *ms)
{
	int		i;

	i = 0;
	while (cmds[0][i] && cmds[0][i] == '.')
		i++;
	if (cmds[0][i] == '\0')
	{
		print_cmd_error(cmds[0], NO_CMD);
		clean_in_child(ms);
		exit(CMD_NF);
	}
}

/**
 * @brief Handles command execution when no PATH variable is set.
 * 
 * Checks if the command exists and has execution permissions. If the file  
 * exists and is executable, it runs with execve. Otherwise, prints an error,  
 * cleans up, and exits.  
 * 
 * @param envp The environment variables.  
 * @param cmd The command and its arguments.  
 * @param ms The minishell struct for cleanup.  
 * 
 * @return This function does not return; it either executes or exits with:  
 *         - `126` if the command lacks execution permissions.  
 *         - `127` if the command does not exist.  
 */
void	handle_no_path_variable(char **envp, char **cmd, t_ms *ms)
{
	if (access(cmd[0], F_OK) == 0)
	{
		if (access(cmd[0], X_OK) == 0)
			execve(cmd[0], cmd, envp);
		print_cmd_error(cmd[0], PERM_DEN);
		clean_in_child(ms);
		exit(CMD_EXEC);
	}
	print_cmd_error(cmd[0], NO_CMD);
	clean_in_child(ms);
	exit(CMD_NF);
}
