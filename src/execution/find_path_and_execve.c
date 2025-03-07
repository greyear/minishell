#include "../../include/minishell.h"

/**
 * @brief Constructs the full path of a command by checking each directory in the PATH.
 * 
 * This function iterates over the directories in the `paths` array, attempting to join each
 * directory with the provided command (`cmd`). It checks if the resulting path exists and
 * is executable. If so, the full path to the command is returned.
 * 
 * @param paths An array of strings representing directories in the system's PATH environment.
 *              Each element in the array is a directory to search for the command.
 * @param cmd A string representing the command name to search for in the directories.
 * 
 * @return The full path to the command if found and executable, or `NULL` if the command is not
 *         found or not executable in any of the directories.
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
        if (access(full_cmd_path, F_OK) == 0
			&& access(full_cmd_path, X_OK) == 0)
            return (full_cmd_path);
        free(full_cmd_path);
        i++;
    }
    return (NULL);
}

/**
 * @brief Checks whether the command is a directory or a file and attempts to execute it.
 * 
 * This function first checks if the provided command is a directory. If it is, an error is printed
 * and the process exits with status 126. If the command is a file, it checks if the file exists and
 * is executable. If both conditions are met, the command is executed using `execve`. If the file is
 * not executable, a permission error is printed, and the process exits with status 126. If the file
 * does not exist, an error message is printed, and the process exits with status 127.
 * 
 * @param envp An array of environment variables used for the `execve` function.
 * @param cmds An array of strings where `cmds[0]` is the command to be checked and executed.
 * 
 * @return This function does not return; it either executes the command or exits the process with
 *         an appropriate error status.
 */

static void	check_if_dir_or_file(char **envp, char **cmds)
{
    DIR *dir;
	
	dir = opendir(cmds[0]);
    if (dir)
    {
        closedir(dir);
        print_cmd_error(cmds[0], IS_DIR);
        exit(126);
    }
    if (access(cmds[0], F_OK) == 0)
    {
        if (access(cmds[0], X_OK) == 0)
            execve(cmds[0], cmds, envp);
        else
        {
			print_cmd_error(cmds[0], PERM_DEN);
            exit(126);
        }
    }
	print_cmd_error(cmds[0], NO_FILE_OR_DIR);
    exit(127);
}

/**
 * @brief Finds the full path of a command by checking the directories listed in the PATH environment variable.
 * 
 * This function looks for the "PATH" variable in the environment variables (`envp`). Once found, it splits
 * the value of "PATH=" into individual directories and attempts to find the full path of the command (`cmds[0]`)
 * in these directories using `make_full_path`. If the command starts with `/` or `.`, it directly checks if
 * it is a directory or file using the `check_if_dir_or_file` function.
 * 
 * @param envp An array of environment variables.
 * @param cmds An array of strings, where `cmds[0]` is the command name whose path needs to be determined.
 * 
 * @return The full path to the command if found, or `NULL` if the command is not found in any directory listed
 *         in the PATH or if the command is an invalid directory/file.
 */

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

/**
 * @brief Checks if a file exists and is executable, then attempts to execute it.
 * 
 * This function checks if the provided command (`cmds[0]`) exists and is executable using `access`. If the
 * command exists and is executable, it is executed with `execve`. If the file exists but is not executable,
 * a permission denied error is printed, and the process exits with status 126. If the file does not exist,
 * an error indicating that the command was not found is printed, and the process exits with status 127.
 * 
 * @param envp An array of environment variables used for the `execve` function.
 * @param cmds An array of strings where `cmds[0]` is the command to be checked and executed.
 * 
 * @return This function does not return; it either executes the command or exits the process with an appropriate
 *         error status.
 */


void    check_if_file(char **envp, char **cmds)
{
    if (access(cmds[0], F_OK) == 0)
    {
        if (access(cmds[0], X_OK) == 0)
            execve(cmds[0], cmds, envp);
        else
        {
			print_cmd_error(cmds[0], PERM_DEN);
            exit(126);
        }
    }
    print_cmd_error(cmds[0], NO_CMD);
    exit(127);
}

/**
 * @brief Finds and executes the command, either by resolving its full path or directly executing the file.
 * 
 * This function checks if a valid command is provided in the `cmd` array. If the command is invalid, it prints an
 * error and exits the process with status 127. It then attempts to find the command's full path using the `find_path_from_envp`
 * function. If the path is found, the command is executed using `execve`. If the path is not found, the function checks if the
 * command is a valid file using `check_if_file`. If the command is not found or not executable, an appropriate error is printed.
 * 
 * @param envp An array of environment variables used for the `execve` function.
 * @param cmd An array of strings, where `cmd[0]` is the command to be executed.
 * 
 * @return This function does not return; it either executes the command or exits the process with an appropriate error status.
 */

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
    check_if_file(envp, cmd);
}