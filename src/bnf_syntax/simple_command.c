
#include "../../include/minishell.h"

/**
 * @brief Validates a simple command in the input string.
 *        `<simple_cmd>	::= {( <redirect> | <word> | "any" | 'any')}`.
 * A simple command consists of any number of redirections, words in quotes or without.
 * The function checks the validity of the command by processing potential redirections and words, allowing for multiple 
 * repetitions of these components.
 * 
 * @param str A pointer to the input string representing the command to be validated.
 * @param err_flag A pointer to an integer flag used to indicate errors. If any validation fails, `err_flag` is set to `1`.
 * 
 * @return A pointer to the next token in the input string after processing the simple command. If an error is encountered,
 *         `err_flag` is set to `1` and the function returns a pointer to the position where the error occurred.
 */
char	*validate_simple_cmd(char *str, int *err_flag)
{
	char	*next;

	//whitespaces can be?
	while (ft_isspace(*str))
		str++;
	next = validate_redirect(str, err_flag);
	if (next == str) //there were no redirect
		next = validate_word(str, err_flag);
	if (*err_flag == 1) // again ")" check
		return (next);
	if (next != str) //we read 1 redirect + cmd or single cmd and need to check for more
		next = validate_simple_cmd(next, err_flag);
	return (next);
}