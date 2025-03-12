
#include "../../include/minishell.h"

/**
 * @brief Merges relevant fields from the second token into the first token.
 * 
 * This function updates `first` by incorporating values from `second`, ensuring that 
 * quote type and unclosed status are properly inherited when applicable.
 * 
 * @param first A pointer to the first token, which will be modified.
 * @param second A pointer to the second token, whose values may be merged into `first`.
 * 
 * @note The function assumes that both tokens are valid and properly initialized.
 */
static void	merged_values(t_token *first, t_token *second)
{
	if (!first->quote && second->quote) // ) + 1 = 1, else we don't need to change the value of the 1st
		first->quote = second->quote;
	if (!first->unclosed && second->quote)
		first->unclosed = true; //check in debugger if we need it
	//some other fields
}

/**
 * @brief Merges consecutive WORD tokens into a single token.
 * 
 * This function traverses a linked list of tokens and combines adjacent tokens of type WORD 
 * into a single token by concatenating their data fields. It also updates relevant token 
 * properties and removes unnecessary whitespace or empty tokens.
 * 
 * @param first A pointer to the first token in the linked list.
 * 
 * @return A pointer to the updated first token of the modified list.
 *         Returns NULL if memory allocation for concatenation fails.
 * 
 * @note This function modifies the linked list in place, freeing redundant tokens 
 *       and updating the structure accordingly.
 */
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
			/*if (cur->data[0] == '$' && cur->quote == 0)
				joined = ft_strdup(cur->next->data);
			else*/ //fixed 309 in parsing hell, but ...
			joined = ft_strjoin(cur->data, cur->next->data);
			if (!joined)
				return (NULL);
			free(cur->data);
			cur->data = joined;
			merged_values(cur, cur->next);
			deleted = cur->next;
			cur->next = cur->next->next;
			clean_token(deleted);
		}
		else
			cur = cur->next;
	}

	/*printf("\n inside uniting: after uniting and before deleting whitespaces\n");
	print_tokens(first);*/

	first = delete_whitespace_tokens(first);
	
	/*printf("\n inside uniting: after deleting whitespaces\n");
	print_tokens(first);*/

	first = delete_empty_word_tokens(first);
	
	/*printf("\n inside uniting: after deleting empty\n");
	print_tokens(first);*/

	return (first);
}
