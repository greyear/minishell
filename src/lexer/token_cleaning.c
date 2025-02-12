
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
			cur->next = cur->next->next; //we deleted next(2), now the next one is next->next(3), next move will be to check it (next for the 1st is 3 now) and delete it if needed
			clean_token(deleted); //[A] → [""] → [""] → [B]
		}
		else
			cur = cur->next; //we go to the next one only if we didn't delete next one
	}
	return (first);
}

t_token	*delete_whitespace_tokens(t_token *first)
{
	t_token	*cur;
	t_token	*deleted;

	if (first && first->type == SPACE)
	{
		deleted = first;
		first = first->next;
		clean_token(deleted);
	}
	cur = first;
	while (cur)
	{
		if (cur->next && cur->next->type == SPACE)
		{
			deleted = cur->next;
			cur->next = cur->next->next;
			clean_token(deleted);
		}
		cur = cur->next; //there's only 1 space token (2) in a row, if we deleted it (2) we go to the next NON-SPACE token (3) and check if (4) is a space
	}
	return (first);
}
