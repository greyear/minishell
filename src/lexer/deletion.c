
#include "../../include/minishell.h"


void	free_stack(t_stack **stack)
{
	t_stack	*next_node;
	t_stack	*cur_node;

	if (!stack)
		return ;
	cur_node = *stack;
	while (cur_node)
	{
		next_node = cur_node->next;
		cur_node->n = 0;
		free(cur_node);
		cur_node = next_node;
	}
	*stack = NULL;
}

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

void	clean_token_list(t_token **first)
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