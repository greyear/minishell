#include "../../include/minishell.h"

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

void	check_if_dir_or_file(char **envp, char **cmds)
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
 * @brief Checks if the command is only a dot (`.`) or starts with multiple dots.
 *
 * This function checks if the first command in `cmds` consists only of dots (`.`).
 * If the condition is met, an error message is printed, and the program exits with a status code of 127, indicating
 * a command not found error.
 * 
 * The function iterates over the characters of the command, ensuring there are no valid commands beyond the dots.
 * 
 * @param cmds A pointer to the array of command strings, with `cmds[0]` being the command to check.
 *
 * @return None. The function does not return a value. If the condition is met, it prints an error and exits the program.
 *
 */

void    check_if_dot(char **cmds)
{
    int     i;

    i = 0;
    while (cmds[0][i] && cmds[0][i] == '.')
        i++;
    if (cmds[0][i] == '\0')
    {
        print_cmd_error(cmds[0], NO_CMD);
        exit(127);
    }
}