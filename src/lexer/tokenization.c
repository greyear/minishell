
#include "../../include/minishell.h"

//str is an input

t_token *tokenization(char *str, t_ms *ms) //store str in ms?
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
	first = unite_two_word_tokens(first);
	if (check_list_for_expansions(first, ms) == 1)
	{
		clean_token_list(&first); //what else to clean?
		return (NULL); //maybe create a separate cleaner?
	}
	return (first);
	//wildcards?
}
