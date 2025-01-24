
#include "../../include/minishell.h"

//- `<simple_cmd>	::= {( <redirect> | <word> | "any" | 'any')}`
//любое количество редиректов ЛИБО слов (в скобках или без)

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