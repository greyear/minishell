
#include "../../include/minishell.h"

void	print_system_error(t_print reason)
{
	ft_putstr_fd(OWN_ERR_MSG, STDERR_FILENO);
	if (reason == HIST_ERR)
		ft_putendl_fd("Reading from history file failed", STDERR_FILENO);
}

void	print_syntax_error(char *text)
{
	int	cut;

	cut = 0;
	ft_putstr_fd(OWN_ERR_MSG, STDERR_FILENO);
	if (*text == NULL_TERM)
		text = "newline";
	else if (ft_strncmp(text, "||", 2) == 0 || ft_strncmp(text, "&&", 2) == 0
		|| ft_strncmp(text, ">>", 2) == 0 || ft_strncmp(text, "<<", 2) == 0)
		cut = 2;
	else if (*text == VERTICAL || *text == L_PARENT || *text == R_PARENT
		|| *text == L_REDIR || *text == R_REDIR || *text == AND)
		cut = 1;
	else if (ft_isalnum(*text) == 1 || *text == SG_QUOT || *text == DB_QUOT)
		while (ft_isalnum(text[cut]) == 1 || text[cut] == SG_QUOT || text[cut] == DB_QUOT)
			cut++;
	//do I need to handle symbols for printing err msg that I don't handle in real parsing??
	if (cut != 0)
		text[cut] = NULL_TERM;
	ft_putstr_fd(": syntax error near unexpected token `", STDERR_FILENO);
	ft_putstr_fd(text, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
}

void	print_file_error(char *file, t_print reason)
{
	ft_putstr_fd(OWN_ERR_MSG, STDERR_FILENO);
	if (reason == HERED_ERR)
	{
		//put msg
	}
	else
		ft_putstr_fd(file, STDERR_FILENO);
	if (reason == NO_FILE)
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	if (reason == PERM_DEN)
		ft_putendl_fd(": Permission denied", STDERR_FILENO);
	if (reason == AMBIG)
		ft_putendl_fd(": ambiguous redirect", STDERR_FILENO);
}

/**
 * @brief Prints an error message for a given command.
 * 
 * This function prints a detailed error message to `stderr` based on the provided
 * error code and command name. It handles different error types like "is a directory",
 * "No such file or directory", "Permission denied", and "command not found".
 * 
 * @param cmd The command name that caused the error.
 * @param c The error code that defines the type of error.
 *          - `IS_DIR`: The command is a directory.
 *          - `NO_FILE_OR_DIR`: The file or directory does not exist.
 *          - `PERM_DEN`: Permission is denied to the command.
 *          - `NO_CMD`: The command was not found.
 * 
 * @return This function does not return a value. It only prints the error message to `stderr`.
 */

void	print_cmd_error(char *cmd, int c)
{
	ft_putstr_fd(OWN_ERR_MSG, STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	if (c == IS_DIR)
		ft_putstr_fd(": is a directory\n", STDERR_FILENO);
	else if (c == NO_FILE_OR_DIR)
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	else if (c == PERM_DEN)
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	else if (c == NO_CMD)
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
}

/**
 * @brief Prints an error message when an invalid identifier is passed to the `unset` command.
 * 
 * This function prints an error message to `stderr` when the user tries to unset an environment
 * variable or identifier that is not valid. The error message includes the argument that caused
 * the error.
 * 
 * @param args The array of arguments passed to the `unset` command.
 * @param i The index of the invalid argument in the `args` array.
 * @param ms The `t_ms` structure containing the exit status and other necessary data.
 * 
 * @return This function does not return a value. It sets the exit status to 1, indicating an error.
 */

void	print_unset_error(char **args, int i, t_ms *ms)
{
	ft_putstr_fd(OWN_ERR_MSG, STDERR_FILENO);
	ft_putstr_fd("unset: '", STDERR_FILENO);
	ft_putstr_fd(args[i], STDERR_FILENO);
	ft_putstr_fd("': not a valid indentifier\n", STDERR_FILENO);
	ms->exit_status = 1;
}

/**
 * @brief Prints an error message when an invalid identifier is passed to the `export` command.
 * 
 * This function prints an error message to `stderr` when the user tries to export an environment
 * variable or identifier that is not valid. The error message includes the argument that caused
 * the error.
 * 
 * @param ms The `t_ms` structure containing the exit status and other necessary data.
 * @param arg The argument passed to the `export` command that caused the error.
 * 
 * @return This function does not return a value. It sets the exit status to 1, indicating an error.
 */

void	print_export_error(t_ms *ms, char *arg)
{
    ft_putstr_fd(OWN_ERR_MSG, STDERR_FILENO);
	ft_putstr_fd("export: '", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	ms->exit_status = 1;
}

/**
 * @brief Prints an error message when the `cd` command fails due to a non-existent directory.
 * 
 * This function prints an error message to `stderr` when the user attempts to change to a directory
 * that does not exist. If a target directory is provided, it will be displayed in the error message.
 * If no target directory is provided, a generic error message will be printed.
 * 
 * @param target_dir The directory that the user tried to change to. If `NULL` or an empty string,
 *        a generic error message is printed.
 * 
 * @return This function does not return a value. It directly prints an error message to `stderr`.
 */

void	print_cd_error(char *target_dir)
{
	ft_putstr_fd(OWN_ERR_MSG, STDERR_FILENO);
	ft_putstr_fd("cd: ", STDERR_FILENO);
	if (target_dir && *target_dir)
		ft_putstr_fd(target_dir, STDERR_FILENO);
	else
		ft_putstr_fd(" ", STDERR_FILENO);
	ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
}

/**
 * @brief Prints an error message when the `env` command encounters a non-existent file or directory.
 * 
 * This function prints an error message to `stderr` when the `env` command is provided with an argument
 * that does not correspond to an existing file or directory. The error message includes the invalid argument
 * that was passed to the `env` command.
 * 
 * @param args The array of arguments passed to the `env` command. The function specifically uses `args[1]`
 *        (the first argument after the command itself) to display the invalid argument in the error message.
 * 
 * @return This function does not return a value. It directly prints an error message to `stderr`.
 */

void	print_env_error(char **args)
{
	ft_putstr_fd(OWN_ERR_MSG, STDERR_FILENO);
	ft_putstr_fd("env: '", STDERR_FILENO);
	ft_putstr_fd(args[1], STDERR_FILENO);
	ft_putstr_fd("': No such file or directory\n", STDERR_FILENO);
}

/**
 * @brief Prints an error message when a non-numeric argument is provided to the `exit` command.
 * 
 * This function is used to print an error message when the `exit` command is called with an argument that
 * is not a valid numeric value. It informs the user that a numeric argument is required to exit the shell.
 * The function uses the argument passed to the `exit` command to display it in the error message.
 * 
 * @param array The array of arguments passed to the `exit` command. The function specifically uses `array[1]`
 *        (the first argument after the command itself) to display the invalid argument in the error message.
 * 
 * @return This function does not return a value. It directly prints an error message to `stderr`.
 */

void	print_numeric_error(char **array)
{
    ft_putstr_fd(OWN_ERR_MSG, STDERR_FILENO);
    ft_putstr_fd("exit: ", STDERR_FILENO);
    ft_putstr_fd(array[1], STDERR_FILENO);
    ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
}

/**
 * @brief Prints an error message when the `exit` command is called with too many arguments.
 * 
 * This function is used to print an error message when the `exit` command is invoked with more than
 * the expected number of arguments. It informs the user that too many arguments were provided and
 * that the `exit` command expects only a numeric argument or no argument at all.
 * 
 * @return This function does not return a value. It directly prints an error message to `stderr`.
 */

void	print_too_many_args_error(void)
{
    ft_putstr_fd(OWN_ERR_MSG, STDERR_FILENO);
    ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
}