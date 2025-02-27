
#include "../../include/minishell.h"

/**
 * @brief Frees memory allocated for a token structure.
 * 
 * This function releases all dynamically allocated memory associated with 
 * a given token, including its `data` and `file` fields.
 * 
 * @param token A pointer to the token structure to be freed.
 */
void	clean_token(t_token *token)
{
	 if (!token)
		return ; //?
	if (token->data)
	{
		free(token->data);
		token->data = NULL;
		//printf("cleaned token->data\n");
	}
	if (token->file)
	{
		free(token->file);
		token->file = NULL;
	}
	//add another fields
	free(token);
	//NULL?
}

/**
 * @brief Frees memory allocated for a linked list of tokens.
 * 
 * This function iterates through the linked list of tokens, freeing each node 
 * and its associated memory. After execution, the pointer to the first token 
 * is set to NULL to prevent dangling references.
 * 
 * @param first A pointer to the head of the token list. The pointer itself 
 *              is set to NULL after all tokens are freed.
 */
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

/**
 * @brief Removes empty word tokens from a linked list of tokens.
 * 
 * This function iterates through the token list and removes any tokens that 
 * are considered empty word tokens. It properly updates the linked list 
 * structure to ensure continuity after deletion.
 * 
 * @param first A pointer to the head of the token list.
 * 
 * @return The updated head of the token list after empty word tokens 
 *         have been removed.
 */
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

/**
 * @brief Removes whitespace tokens from a linked list of tokens.
 * 
 * This function iterates through the token list and removes any tokens 
 * that represent spaces. It ensures that the linked list 
 * structure remains valid after deletion.
 * 
 * @param first A pointer to the head of the token list.
 * 
 * @return The updated head of the token list after whitespace tokens 
 *         have been removed.
 */
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
