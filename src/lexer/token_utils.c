
#include "../../include/minishell.h"

t_bool	is_empty_word_token(t_token *token)
{
	if (token && token->type == WORD) //check r
	{
		if (token->quote == 0 && (!token->data || !token->data[0]))
			return (1); //try both with and without quotes
	}
	return (0);
}
