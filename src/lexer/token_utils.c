
#include "../../include/minishell.h"

/**
 * @brief Checks if a given token is an empty WORD token.
 * 
 * A token is considered an empty WORD if its type is WORD and:
 * - It has no assigned data (NULL or an empty string).
 * - It is not enclosed in quotes.
 * 
 * This function is useful for filtering out unnecessary tokens before further 
 * processing.
 * 
 * @param token A pointer to the t_token structure to be checked.
 * 
 * @return true (1) if the token is an empty WORD, false (0) otherwise.
 */
t_bool	is_empty_word_token(t_token *token)
{
	//check if token
	if (token && token->type == WORD) //check r
	{
		if (token->quote == 0 && (!token->data || !token->data[0]))
			return (1); //try both with and without quotes
	}
	return (0);
}
