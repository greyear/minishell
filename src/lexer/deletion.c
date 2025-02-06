
#include "../../include/minishell.h"

void	clean_token(t_token *token)
{
	if (token->data)
		free(token->data);
	if (token->file)
		free(token->file);
	//add another fields
	free(token);
	//NULL?
}

void	clean_token_list(t_token **first) //change it to return NULL?
{
	t_token	*cur;
	t_token	*next;

	if (!first || !*first)
		return ;
	cur = *first;
	while (cur)
	{
		next = cur->next;
		clean_token(cur);
		cur = next;
	}
	*first = NULL;
}

t_token	*delete_empty_word_tokens(t_token *first)
{
	t_token	*cur;
	t_token	*deleted;

	if (is_empty_word_token(first))
	{
		deleted = first;
		first = first->next;
		clean_token(deleted);
	}
	cur = first;
	while (cur)
	{
		if (is_empty_word_token(cur->next))
		{
			deleted = cur->next;
			cur->next = cur->next->next;
			clean_token(deleted);
		}
		else
			cur = cur->next;
	}
	return (first);
}
