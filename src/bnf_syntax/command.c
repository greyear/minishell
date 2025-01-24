
#include "../../include/minishell.h"

//- `<command>	::= <simple_cmd> | '(' <pipeline> ')' {<redirect>}`
//у нас есть ЛИБО симпл команд, ЛИБО пайплайн + редирект

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
