#include "../../include/minishell.h"

/**
 * @brief Handles the execution of commands given as absolute or relative paths.
 * 
 * This function checks whether the provided command is a directory, an 
 * existing file, or an executable. If it is a directory, it prints an error 
 * and exits. If it exists but lacks execution permissions, it prints an error 
 * and exits. If the command does not exist, it prints an error and exits.
 * 
 * @param envp The environment variables.
 * @param cmds The command and its arguments.
 * 
 * @return This function does not return; it either executes the command or 
 *         exits the process with:
 *         - `126` if the command is a directory or lacks execution 
 *           permissions.
 *         - `127` if the command does not exist.
 */
void	handle_absolute_or_relative_path(char **envp, char **cmds)
{
	DIR	*dir;

	dir = opendir(cmds[0]);
	if (dir)
	{
		closedir(dir);
		print_cmd_error(cmds[0], IS_DIR);
		exit(CMD_EXEC);
	}
	if (access(cmds[0], F_OK) == 0)
	{
		if (access(cmds[0], X_OK) == 0)
			execve(cmds[0], cmds, envp);
		print_cmd_error(cmds[0], PERM_DEN);
		exit(CMD_EXEC);
	}
	print_cmd_error(cmds[0], NO_FILE_OR_DIR);
	exit(CMD_NF);
}

/**
 * @brief Checks if the command is only a dot (`.`) or starts with multiple dots.
 *
 * This function checks if the first command in `cmds` consists only of dots 
 * (`.`). If the condition is met, an error message is printed, and the 
 * program exits with a status code of 127, indicating a command not found 
 * error.
 * 
 * The function iterates over the characters of the command, ensuring there 
 * are no valid commands beyond the dots.
 * 
 * @param cmds A pointer to the array of command strings, with `cmds[0]` 
 *             being the command to check.
 *
 * @return None. The function does not return a value. If the condition is 
 *         met, it prints an error and exits the program.
 */
void	check_if_dot(char **cmds)
{
	int		i;

	i = 0;
	while (cmds[0][i] && cmds[0][i] == '.')
		i++;
	if (cmds[0][i] == '\0')
	{
		print_cmd_error(cmds[0], NO_CMD);
		exit(CMD_NF);
	}
}

/**
 * @brief Handles the case when the `PATH` environment variable is not set and 
 *        attempts to execute a command directly.
 * 
 * This function checks if the command provided as the first argument (`cmd[0]`) 
 * exists and is executable. If the command exists and has execute permissions, 
 * it calls `execve` to run the command. If the command exists but does not have 
 * execute permissions, it prints a permission denied error and exits with 
 * status 126. If the command does not exist, it prints a "command not found"
 * error and exits with status 127.
 * 
 * @param envp An array of environment variables, which is passed to `execve`.
 * @param cmd An array of command arguments, where `cmd[0]` is the command to 
 *            be executed.
 * 
 * @return This function does not return. If the command is not found or 
 *         executable, the program exits with an appropriate status code 
 *         (127 for command not found, 126 for permission denied).
 */
void	handle_no_path_variable(char **envp, char **cmd)
{
	if (access(cmd[0], F_OK) == 0)
	{
		if (access(cmd[0], X_OK) == 0)
			execve(cmd[0], cmd, envp);
		print_cmd_error(cmd[0], PERM_DEN);
		exit(CMD_EXEC);
	}
	print_cmd_error(cmd[0], NO_CMD);
	exit(CMD_NF);
}
