
#include "../../include/minishell.h"

char	*word_with_quotes(char *str, size_t *start, t_token *new)
{
	char	*res;
	size_t	end;

	//str protection?
	*start = *start + 1;
	end = *start;
	while (str[end] && ((new->quote == SG_QUOT && str[end] != SG_QUOT)
			|| (new->quote == DB_QUOT && str[end] != DB_QUOT)))
		end++;
	if (str[end] == '\0') //do we need the check for unclosed here also?
		new->unclosed = true; //нужно ли здесь сразу выйти?
	res = (char *)ft_calloc((end - *start + 1), sizeof(char));
	if (!res)
		return (NULL);
	ft_strlcpy(res, str + *start, end - *start + 1);
	*start = end;
	if (new->unclosed == false)
		*start = *start + 1;
	return (res);
}
char	*word_without_quotes(char *str, size_t *start)
{
	char	*res;
	size_t	end;

	//str protection?
	end = *start;
	while (str[end] && !ft_special(str[end]) && !ft_isspace(str[end]))
		end++;
	res = (char *)ft_calloc((end - *start + 1), sizeof(char));
	if (!res)
		return (NULL);
	ft_strlcpy(res, str + *start, end - *start + 1);
	*start = end;
	return (res);
}
