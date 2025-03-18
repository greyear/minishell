
#include "../../include/minishell.h"
#include <stdint.h> //delete

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
		return (0);
	next = validate_pipeline(str, &err_flag);
	/*if (next) {
		printf("DEBUG: next[0] = %c\n", next[0]);
		printf("DEBUG: next[1] = %c\n", next[1]);
	}*/


	/*printf("err_flag: '%d'\n", err_flag);
	if (next[0] == "")
		printf("YES, NULL_TERM\n");*/
	if (err_flag == 1 || *next != NULL_TERM) //line DOESN'T end with \0
	{
		//printf("next: '%s'\n", next);
		print_syntax_error(next);
		return (SYNTAX_ERR); // code?
	}
	return (0);
}
