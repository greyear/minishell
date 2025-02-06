
#include "../../include/minishell.h"

t_token *tokenization(char *str)
{
	size_t			i;
	t_token_type	type;
	t_token			*first;
	t_token			*cur;

	//where's the check for \0 str?
	i = 0;
	first = create_new_token(NULL, &i, DUMMY); //we're passing NULL instead of str cause:
	//it's just a dummy node and I wanna stress that it's not logically connected
	if (!first)
		return (NULL);
	cur = first;
	while (str[i])
	{
		type = define_token_type(str, i);
		cur->next = create_new_token(str, &i, type); //where i increases
		if (!cur->next)
		{
			clean_token_list(&first);
			return (NULL);
		}
		cur = cur->next;
	}
	flags_for_redirections(first);
	return (first);
	//something about expanding $HOME
	// quotes handling
}

/*
is_empty - empty words token
clean_empty_strings - deletes empty nodes from the list
clean_whitepace - deletes whitespace nodes (use for the 2nd node after merging)
add_up_values - preparation for merging of tokens
clean_quotes_and_whitespaces - if both tokens are words: joins words, deletes 2nd node,


*/
