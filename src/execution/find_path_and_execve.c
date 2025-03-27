/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path_and_execve.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 13:52:28 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 13:52:30 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Constructs the full path of a command by combining directories
 *        from PATH.
 * 
 * This function takes an array of directories (from the "PATH" environment 
 * variable) and attempts to find the full path to the given command by 
 * checking each directory for the command. If the command is found, the full 
 * path is returned. Memory is properly managed by freeing intermediate strings.
 * 
 * @param paths An array of directories (extracted from the "PATH" environment 
 *              variable).
 * @param cmd The command to search for in the directories.
 * @param ms A pointer to the main shell structure, used to handle errors and 
 *           status.
 * 
 * @return The full path to the command if found, or NULL if not found or if 
 *         an error occurs.
 */
static char	*make_full_path(char **paths, char *cmd, t_ms *ms)
{
	int		i;
	char	*new_full_path;
	char	*full_cmd_path;

	i = -1;
	while (paths[++i])
	{
		new_full_path = ft_strjoin(paths[i], "/");
		if (!new_full_path)
			return (print_malloc_set_status(ms));
		full_cmd_path = ft_strjoin(new_full_path, cmd);
		free(new_full_path);
		if (!full_cmd_path)
			return (print_malloc_set_status(ms));
		if (access(full_cmd_path, F_OK) == 0)
			return (full_cmd_path);
		free(full_cmd_path);
	}
	return (NULL);
}

/**
 * @brief Finds the full path of a command by searching the PATH 
 *        environment variable.
 * 
 * This function iterates through the environment variables to find the "PATH" 
 * variable. It then splits the value of "PATH" into individual directories, 
 * searching each for the specified command. The function returns the full path 
 * of the command if found. If memory allocation fails or the "PATH" variable 
 * is empty, it handles errors accordingly.
 * 
 * @param envp A pointer to the environment variables.
 * @param cmds A pointer to the array of command arguments (the first argument 
 *             is the command to search for).
 * @param ms A pointer to the main shell structure, used to handle errors 
 *           and status.
 * 
 * @return The full path to the command if found, or NULL if not found or 
 *         if an error occurs.
 */
static char	*find_path_from_envp(char **envp, char **cmds, t_ms *ms)
{
	int		i;
	char	*path_var;
	char	**paths;
	char	*full_path;

	i = -1;
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_var = envp[i] + 5;
			if (path_var[0] == '\0')
				return (NULL);
			paths = ft_split(path_var, ':');
			if (!paths)
				return (print_malloc_set_status(ms));
			full_path = make_full_path(paths, cmds[0], ms);
			clean_arr(&paths);
			return (full_path);
		}
	}
	return (NULL);
}

/**
 * @brief Executes a command by searching for its path and calling `execve`.
 * 
 * This function checks if the provided command is valid, resolves its path 
 * (either through the `PATH` environment variable or directly if it's an 
 * absolute/relative path), and then attempts to execute it using `execve`. 
 * It also handles errors related to command not found, permission denied, 
 * or memory allocation failure.
 * 
 * @param envp A pointer to the environment variables.
 * @param cmd A pointer to the command arguments.
 * @param ms A pointer to the main shell structure containing shell state.
 * 
 * @return This function does not return; it exits the program after attempting 
 *         to execute the command.
 */
void	execute_command(char **envp, char **cmd, t_ms *ms)
{
	char	*path;

	check_if_dot(cmd);
	if (cmd[0][0] == '/' || cmd[0][0] == '.')
		handle_absolute_or_relative_path(envp, cmd);
	if (!get_env_value("PATH", envp))
		handle_no_path_variable(envp, cmd);
	path = find_path_from_envp(envp, cmd, ms);
	if (ms->exit_status == MALLOC_ERR)
		exit(MALLOC_ERR);
	if (!path)
	{
		print_cmd_error(cmd[0], NO_CMD);
		exit(CMD_NF);
	}
	execve(path, cmd, envp);
	print_cmd_error(path, PERM_DEN);
	free(path);
	exit(CMD_EXEC);
}
