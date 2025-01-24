
#include "../../include/minishell.h"

//- `<pipeline>	::= <command> { '|' <command> }`
char	*validate_pipeline(char *str, int *err_flag)
{
	char	*next;

	while (ft_isspace(*str))
		str++;
	next = validate_cmd(str, err_flag);
	if (next == str) //не нашли команду
		*err_flag = 1;
	if (*err_flag == 1) //+ слово с незакрытой кавычкой, редирект без файла или со спецсимволом вместо файла
		return (next);
	if (*next == PIPE && *(next + 1) != PIPE) //проверка, что не ||
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