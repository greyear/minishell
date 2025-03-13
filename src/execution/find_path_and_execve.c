#include "../../include/minishell.h"

/**
 * @brief Constructs the full path of the command by combining directories from the `PATH` variable and the command name.
 * 
 * This function iterates over each directory in the `paths` array (extracted from the `PATH` environment variable) and 
 * attempts to combine it with the command name (`cmd`) to create a potential full path. It checks if the constructed 
 * full path exists using `access()`. If it finds an existing file at that path, it returns the full path. If no valid path 
 * is found after checking all directories, it returns `NULL`.
 * 
 * @param paths An array of directory paths from the `PATH` environment variable.
 * @param cmd The command name to search for within the directories in `paths`.
 * 
 * @return Returns the full path to the command if it exists, otherwise returns `NULL`.
 */

static char	*make_full_path(char **paths, char *cmd)
{
	int		i;
	char	*new_full_path;
	char	*full_cmd_path;

	i = 0;
	while (paths[i])
	{
		new_full_path = ft_strjoin(paths[i], "/");
		if (!new_full_path)
			return (NULL);
		full_cmd_path = ft_strjoin(new_full_path, cmd);
		free(new_full_path);
		if (!full_cmd_path)
			return (NULL);
		if (access(full_cmd_path, F_OK) == 0)
			return (full_cmd_path);
		free(full_cmd_path);
		i++;
	}
	return (NULL);
}

/**
 * @brief Searches the `PATH` environment variable for the specified command.
 * 
 * This function iterates through the environment variables to find the `PATH` variable, which contains a colon-separated 
 * list of directories. It then attempts to find the command specified in `cmds[0]` by searching through the directories 
 * listed in `PATH`. If the command is found, it returns the full path to the command. If the command is not found or 
 * `PATH` is not set, it returns `NULL`.
 * 
 * @param envp An array of environment variables, including the `PATH` variable.
 * @param cmds An array of command arguments, where `cmds[0]` is the command name to search for.
 * 
 * @return Returns the full path to the command if found, otherwise returns `NULL`.
 */

static char *find_path_from_envp(char **envp, char **cmds)
{
	int		i;
	char	*path_var;
	char	**paths;
	char	*full_path;

	i = 0;
	full_path = NULL;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_var = envp[i] + 5;
			if (path_var[0] == '\0')
				return (NULL);
			paths = ft_split(path_var, ':');
			full_path = make_full_path(paths, cmds[0]);
			clean_arr(&paths);
			break;
		}
		i++;
	}
	return (full_path);
}

/**
 * @brief Executes a command by searching for its path and running it with `execve()`.
 * 
 * This function processes and executes a given command by checking its validity, 
 * resolving its path, and executing it. If the command is an absolute or relative path, 
 * it is validated using `handle_absolute_or_relatve_path()`. If no `PATH` variable is found, 
 * it handles execution using `handle_no_path_variable()`. Otherwise, the function 
 * searches for the command in the system's `PATH`, executes it if found, or 
 * prints an error if it cannot be executed.
 * 
 * @param envp The environment variables.
 * @param cmd An array of strings representing the command and its arguments.
 * 
 * @return This function does not return; it either executes the command or exits the process with:
 *         - `127` if the command is not found.
 *         - `126` if the command exists but lacks execution permission.
 */

void    execute_command(char **envp, char **cmd)
{
	char	*path;

	if (!cmd || !*cmd)
	{
		print_cmd_error(NULL, 0);
		exit(127);
	}
	check_if_dot(cmd);
	if (cmd[0][0] == '/' || cmd[0][0] == '.')
		handle_absolute_or_relative_path(envp, cmd);
	if (!get_env_value("PATH", envp))
		handle_no_path_variable(envp, cmd);
	path = find_path_from_envp(envp, cmd);
	if (!path)
	{
		print_cmd_error(cmd[0], NO_CMD);
		exit(127);
	}
	//printf("signal before execve %d\n ", g_sgnl);
	execve(path, cmd, envp);
	print_cmd_error(path, PERM_DEN);
	free(path);
	exit(126);
}