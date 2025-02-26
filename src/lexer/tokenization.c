
#include "../../include/minishell.h"

//str is an input

/**
 * @brief Tokenizes an input string into a linked list of tokens.
 * 
 * This function processes a given command string, splitting it into tokens based on 
 * spaces, special characters (e.g. `<`, `>`, `|`), and quotes. It assigns each 
 * token a specific type and applies necessary post-processing such as handling 
 * redirections and merging adjacent words.
 * 
 * @param str The null-terminated input string to be tokenized.
 * @param ms A pointer to the t_ms structure, potentially used for expansions.
 * 
 * @return A pointer to the first token in the linked list, or NULL if an error occurs.
 * 
 * @note The function allocates memory dynamically. The caller is responsible for 
 *       freeing the token list using clean_token_list().
 */
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
	/*printf("inside tokenization before the redirections\n");
	print_tokens(first);*/

	flags_for_redirections(first);

	/*printf("inside tokenization before uniting\n");
	print_tokens(first);*/

	first = unite_two_word_tokens(first);
	
	/*printf("inside tokenization before the expantion\n");
	print_tokens(first);*/

	if (check_list_for_expansions(first, ms) == 1)
	{
		clean_token_list(&first); //what else to clean?
		return (NULL); //maybe create a separate cleaner?
	}
	/*printf("inside tokenization end\n");
	print_tokens(first);*/
	//print_tokens(first);
	return (first);
	//wildcards?
}
