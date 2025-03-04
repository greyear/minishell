/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path_and_execute_command.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 15:38:13 by ssalorin          #+#    #+#             */
/*   Updated: 2025/02/21 18:51:11 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
ALWAYS CALL FT_PIPE FIRST EVEN WITH SINGLE COMMAND; SO IT FORKS THE PROCESS AND THEN TRIES TO EXECUTE


FT_FIND_PATH_2 FUNCTION:
Purpose: This function tries to find the full path of a command
by appending the command to each directory in the PATH environment variable.
It iterates through each directory path in paths[].
It constructs the full path by concatenating the directory with the command name.
It checks if the command exists and is executable using access(new_full_path, X_OK).
If found, it returns the full path of the command. Otherwise, it continues the search.

FT_FIND_PATH FUNCTION:
It first checks if the command starts with a /, meaning it is an absolute path.
If so, it checks if the file exists and is executable.
Then it searches the envp[] array for the PATH variable.
If PATH is found, it splits the directories in PATH (using ft_split) and calls ft_find_path2 to try to locate the command.
If the command is not found, it handles the error by printing an error message and exiting.

FT_CHECK_DIR FUNCTION:
Purpose: This function checks if the command refers to a directory (e.g., ./some_dir/) and handles it.

FT_IF_NOT_PATH FUNCTION:
Purpose: This function handles the case where the command cannot be found.

FT_COMMAND FUNCTON:
It checks if the command is empty or consists only of spaces, in which case it prints an error and exits.
It splits the command string into individual arguments using ft_split.
It then finds the full path of the command using ft_find_path.
If the command is not found, it calls ft_if_not_path.
It uses execve to execute the command at the found path with the given arguments (cmds).
If execve fails, it frees resources and exits with failure.
*/

//#include "seela.h"
#include "../../include/minishell.h"
#include "../../include/minishell.h"

static char	*make_full_path(char **paths, char *full_path, char *cmd)
{
	int		i;
	char	*temp_path;
	char	*new_full_path;

	i = 0;
	temp_path = NULL;
	new_full_path = NULL;
	while (paths[i])
	{
		temp_path = ft_strjoin(paths[i], "/");
		if (!temp_path)
		{
			free(full_path);
			return (NULL);
		}
		new_full_path = ft_strjoin(temp_path, cmd);
		free(temp_path);
		if (access(new_full_path, X_OK) == 0)
		{
			free(full_path);
			return (new_full_path);
		}
		free(new_full_path);
		i++;
	}
	free(full_path);
	return (NULL);
}

void	check_if_dir(char **envp, char **cmds)
{
	DIR *dir;
	
	dir = opendir(cmds[0]);
    if (dir) // If it's a directory
    {
        closedir(dir);
        ft_putstr_fd(OWN_ERR_MSG, 2);
        ft_putstr_fd(cmds[0], 2);
        ft_putstr_fd(": Is a directory\n", 2);
        exit(126);
    }
    if (access(cmds[0], F_OK) == 0) // File exists
    {
        if (access(cmds[0], X_OK) == 0) // File is executable
            execve(cmds[0], cmds, envp);
        else
        {
            ft_putstr_fd(OWN_ERR_MSG, 2);
            ft_putstr_fd(cmds[0], 2);
            ft_putstr_fd(": Permission denied\n", 2);
            exit(126);
        }
    }
    ft_putstr_fd(OWN_ERR_MSG, 2);
    ft_putstr_fd(cmds[0], 2);
    ft_putstr_fd(": No such file or directory\n", 2);
    exit(127);
}

static char	*find_path_from_envp(char **envp, char **cmds)
{
	int		i;
	char	*path_var;
	char	**paths;
	char	*full_path;

	i = 0;
	full_path = NULL;
	if (cmds[0][0] == '/' || cmds[0][0] == '.')
		check_if_dir(envp, cmds);
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
	{
		print_cmd_error(cmds[0], 2);
		exit(127);
	}
	path_var = envp[i] + 5;
	if (envp[i][5] == '\0')
	{
		print_cmd_error(cmds[0], 1);
		exit(126);
	}
	paths = ft_split(path_var, ':');
	full_path = make_full_path(paths, full_path, cmds[0]);
	clean_arr(&paths);
	return (full_path);
}

static void	check_dir(char *cmd)
{
	int		len;

	len = ft_strlen(cmd) - 1;
	if (cmd[0] == '.' && cmd[len] == '/')
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		exit(126);
	}
}

static void	if_not_path(char **cmds)
{
	int	x;

	x = 0;
	check_dir(cmds[0]);
	print_cmd_error(cmds[0], 0);
	if (cmds[0][0] == '.')
		x = 1;
	if (x == 1)
		exit(126);
	exit(127);
}

void	execute_command(char **envp, char **cmd)
{
	char	*path;

	if (!cmd || !*cmd)
	{
		print_cmd_error(NULL, 0);
		exit(127);
	}
	path = find_path_from_envp(envp, cmd);
	if (!path)
		if_not_path(cmd);
	execve(path, cmd, envp);
	free(path);
	exit(EXIT_FAILURE);
}
