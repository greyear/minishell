
#include "../../include/minishell.h"

static void	merged_values(t_token *first, t_token *second)
{
	if (!first->quote && second->quote) // ) + 1 = 1, else we don't need to change the value of the 1st
		first->quote = second->quote;
	if (!first->unclosed && second->quote)
		first->unclosed = true; //check in debugger if we need it
	//some other fields
}

t_token	*unite_two_word_tokens(t_token *first)
{
	t_token	*cur;
	t_token	*deleted;
	char	*joined;

	//check r
	cur = first;
	while (cur)
	{
		if (cur->next && cur->type == WORD && \
				cur->next->type == WORD)
		{
			joined = ft_strjoin(cur->data, cur->next->data);
			if (!joined)
				return (NULL);
			cur->data = joined;
			merged_values(cur, cur->next);
			deleted = cur->next;
			cur->next = cur->next->next;
			clean_token(deleted);
		}
		else
			cur = cur->next;
	}
	//print_tokens(first);
	first = delete_whitespace_tokens(first);
	//print_tokens(first);
	first = delete_empty_word_tokens(first);
	//print_tokens(first);
	return (first);
}
