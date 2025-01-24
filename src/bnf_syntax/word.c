
#include "../../include/minishell.h"

/*
<word> — это блок, который может содержать текстовые символы.
Может встречаться в составе команды или редиректа.
<word> | "any" | 'any'
интерпретируем слова как без, так и в кавычках!

Специальные символы не могут являться частью блока word, они должны
его прерывать. Например в "echo file|name" | должно быть
 интерпретировано как пайплайн, а не часть <word>.

Всё, что внутри кавычек, считается валидным содержимым 
<word> (даже специальные символы).

*/

static int	ft_special(int c)
{
	if (c == '|' || c == '&' || c == '<' || c == '>' || \
				c == '(' || c == ')' || c == ' '|| \
				c == '\'' || c == '\"')
		return (1);
	return (0);
}

static int	length_inside_quotes(char *str, int *err_flag)
{
	char	any_quote;
	int		len;

	any_quote = *str;
	len = 0;
	str++;
	while (*str != '\0' && *str != any_quote)
	{
		len++;
		str++;
	}
	if (*str != any_quote)
		*err_flag = 1;
	return (len);
}

/*Возвращаем:
- если просто слово - нулл-терминатор
- если слово в кавычках (даже со спецсимволами внутри) - первый символ после кавычки
- если слово с незакрытой кавычкой - нулл-терминатор и статус фэйл
- если спецсимвол - его*/

char	*validate_word(char *str, int *err_flag)
{
	int	len_inside;

	while (*str && !ft_special(*str))
		str++;
	if (*str == '\'' || *str == '\"')
		str += length_inside_quotes(str, err_flag);
	return (str);
}
