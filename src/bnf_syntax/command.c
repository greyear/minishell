
#include "../../include/minishell.h"

/**
 * @brief Validates a command, which can be a simple command or a pipeline with an optional redirect.
 *        `<command> ::= <simple_cmd> | '(' <pipeline> ')' {<redirect>}`.
 * 
 * @param str A pointer to the null-terminated input string to be validated.
 * @param err_flag A pointer to an integer flag used to indicate errors. If an error is detected, 
 *                 the flag is set to `1`.
 * 
 * @return A pointer to the next token in the input string after processing the command. If an error 
 *         is encountered, `err_flag` is set to `1` and the function returns a pointer to the position 
 *         where the error occurred.
 */
char	*validate_cmd(char *str, int *err_flag);

static char	*pipeline_redirect(char *str, int *err_flag)
{
	char	*next;

	str++;
	if (ft_isemptystring(str))
	{
		*err_flag = 1;
		return (str);
	}
	next = validate_pipeline(str, err_flag);
	if (*err_flag == 1)
		return (next);
	if (*next != R_PARENT)
	{
		*err_flag = 1;
		return (next);
	}
	next++;
	while (ft_isspace(*next))
		next++;
	next = validate_redirect(next, err_flag);
	return (next);
}

char	*validate_cmd(char *str, int *err_flag)
{
	char	*next;

	next = validate_simple_cmd(str, err_flag);
	if (next != str || *err_flag == 1) //1) we found simple cmd and finish
		return (next); //2) слово с незакрытой кавычкой, редирект без файла или со спецсимволом вместо файла
	else if (*str == L_PARENT)
		next = pipeline_redirect(str, err_flag);
	return (next);
}
