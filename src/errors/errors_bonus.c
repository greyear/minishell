#include "../../include/minishell.h"

void	print_system_error(t_print reason)
{
	ft_putstr_fd(OWN_ERR_MSG, STDERR_FILENO);
	if (reason == HIST_ERR)
		ft_putendl_fd("Reading from history file failed", STDERR_FILENO);
}

/**
 * @brief Prints a syntax error message for unexpected tokens.
 * 
 * This function formats and displays an error message when a syntax 
 * error is encountered. It identifies unexpected tokens, such as
 * operators or special characters, and ensures proper formatting 
 * of the error message. If the token is empty, "newline" is used instead.
 * 
 * @param text The unexpected token causing the syntax error.
 */
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
	if (reason == DIRECT)
		ft_putendl_fd(": Is a directory", STDERR_FILENO);
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
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
	else if (c == NO_FILE_OR_DIR)
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	else if (c == PERM_DEN)
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	else if (c == NO_CMD)
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
}