
#include "../../include/minishell.h"

void	skip_special_tokens(char *str, size_t *i, t_token_type type)
{
	if (!str || !i)
		return;
	while (str[*i])
	{
		if (type == HEREDOC || type == APPEND)
			(*i) += 2;
		else if (type != WORD && type != SPACE)
			(*i)++;
	}
}

void	skip_whitespaces(char *str, size_t *i)
{
	if (!str || !i)
		return;
	while (str[*i] && ft_isspace(str[*i]))
		(*i)++;
}
