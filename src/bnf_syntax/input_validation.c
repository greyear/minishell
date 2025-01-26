
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

	//что я делаю, если вводят \n?
	err_flag = 0;
	while (ft_isspace(*str))
		str++;
	if (*str == NULL_TERM)
		return (0); //empty line is handled as success
	next = validate_pipeline(str, &err_flag);
	if (err_flag == 1 || *next != NULL_TERM) //line DOESN'T end with \0
	{
		//error message
		return (1); // code?
	}
	return (0);
}
