
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
