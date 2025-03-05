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

#include "../../include/minishell.h"

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
        if (access(full_cmd_path, F_OK) == 0
			&& access(full_cmd_path, X_OK) == 0)
            return full_cmd_path;
        free(full_cmd_path);
        i++;
    }
    return (NULL);
}

static void	check_if_dir_or_file(char **envp, char **cmds)
{
    DIR *dir;
	
	dir = opendir(cmds[0]);
    if (dir)
    {
        closedir(dir);
        print_cmd_error(cmds[0], 3);
        exit(126);
    }
    if (access(cmds[0], F_OK) == 0)
    {
        if (access(cmds[0], X_OK) == 0)
            execve(cmds[0], cmds, envp);
        else
        {
			print_cmd_error(cmds[0], 1);
            exit(126);
        }
    }
	print_cmd_error(cmds[0], 2);
    exit(127);
}

static char *find_path_from_envp(char **envp, char **cmds)
{
	int		i;
	char	*path_var;
    char	**paths;
    char	*full_path;

	i = 0;
	full_path = NULL;
	if (cmds[0][0] == '/' || cmds[0][0] == '.')
        check_if_dir_or_file(envp, cmds);
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

void	execute_command(char **envp, char **cmd)
{
	char	*path;
    if (!cmd || !*cmd)
    {
        print_cmd_error(NULL, 0);
        exit(127);
    }
    path = find_path_from_envp(envp, cmd);
    if (path)
    {
        execve(path, cmd, envp);
        free(path);
    }
    check_if_dir_or_file(envp, cmd);
}
