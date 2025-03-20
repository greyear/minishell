
//mallocs checked

#include "../../include/minishell.h"

/**
 * @brief Validates the input string according to the BNF rule:
 *        `<v_input> ::= <pipeline> <newline>`.
 * 
 * @param str A pointer to the input string (null-terminated) to be validated.
 * 
 * @return int Returns `0` if the input is valid, or `1` if it is invalid.
 */
int	validate_input(char *str)
{
	char	*next;
	int		err_flag;

	err_flag = 0;
	while (ft_isspace(*str))
		str++;
	if (*str == NULL_TERM)
		return (0);
	next = validate_pipeline(str, &err_flag);
	if (err_flag == 1 || *next != NULL_TERM)
	{
		print_syntax_error(next);
		return (SYNTAX_ERR);
	}
	return (0);
}
