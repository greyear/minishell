
#include "../../include/minishell.h"

//- `<redirect>	::= ( '<' | '>' | '<<' | '>>' ) <word>`
char	*validate_redirect(char *str, int *err_flag)
{
	char	*next;

	//всегда должно начинаться с < or >!! плюс слово (файл)
	if (str[0] != '<' && str[0] != '>')
		return (str);
	else if (ft_strncmp(str, "<<", 2) == 0 || \
			ft_strncmp(str, ">>", 2) == 0)
		str += 2;
	else if (ft_strncmp(str, "<", 1) == 0 || \
			ft_strncmp(str, ">", 1) == 0)
		str += 1;
	while (ft_isspace(*str))
		str++;
	/*здесь пропускаем имя файла*/
	next = validate_word(str, err_flag);
	if (next == str) //null-terminator or special symbol
		*err_flag = 1;
	//2 previuos cases + unclosed quote:
	if (*err_flag == 1) //Dima also has ")" symbol check here, why?
		return (next);
	while (ft_isspace(*next))
		next++;
	//recursive call of the same function which will check all the redirections if there're many
	next = validate_redirect(next, err_flag);
	return (next);
}
