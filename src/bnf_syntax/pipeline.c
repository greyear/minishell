
//mallocs checked

#include "../../include/minishell.h"

/**
 * @brief Validates a pipeline of commands separated by pipes (`|`):
 *        `<pipeline> ::= <command> { '|' <command> }`.
 * 
 * @param str A pointer to the input string to be validated.
 * @param err_flag A pointer to an integer flag used to indicate errors. If any validation fails, `err_flag` is set to `1`.
 * 
 * @return A pointer to the next token in the input string after processing the pipeline. If an error is encountered,
 *         `err_flag` is set to `1` and the function returns a pointer to the position where the error occurred.
 */
char	*validate_pipeline(char *str, int *err_flag)
{
	char	*next;

	while (ft_isspace(*str))
		str++;
	next = validate_cmd(str, err_flag);
	if (next == str)
		*err_flag = 1;
	if (*err_flag == 1)
		return (next);
	if (*next == VERTICAL && *(next + 1) != VERTICAL)
	{
		next++;
		if (ft_isemptystring(next))
		{
			*err_flag = 1;
			return (next);
		}
		next = validate_pipeline(next, err_flag);
	}
	return (next);
}