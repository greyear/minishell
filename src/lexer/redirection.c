
//mallocs checked

#include "../../include/minishell.h"

/**
 * @brief Checks if a given token type represents a redirection operator.
 * 
 * This function determines whether the specified token type corresponds to 
 * a redirection operator, such as input (`<`), output (`>`), heredoc (`<<`), 
 * or append (`>>`).
 * 
 * @param type The token type to be checked.
 * 
 * @return `true` if the token type is a redirection operator, otherwise `false`.
 */
t_bool	is_redirect(t_token_type type)
{
	if ((type == IN) || (type == OUT) || \
			(type == HEREDOC) || (type == APPEND))
		return (true);
	return (false);
}

/**
 * @brief Assigns redirection flags to tokens following redirection operators.
 * 
 * This function scans a linked list of tokens and assigns a specific redirection 
 * type to words that follow redirection operators (`<`, `>`, `<<`, `>>`). 
 * It ensures that the correct redirection flag is set while handling spaces 
 * and edge cases like consecutive redirections or concatenated words.
 * 
 * @param cur A pointer to the first token in the list.
 */
void	flags_for_redirections(t_token *cur)
{
	t_token_type	specific;

	specific = EMPTY;
	while (cur)
	{
		if (is_redirect(cur->type))
		{
			specific = cur->type;
			cur = cur->next;
			continue ;
		}
		if (is_redirect(specific))
		{
			if (cur && cur->type == SPACE)
				cur = cur->next;
			while (cur && cur->type == WORD)
			{
				cur->specific_redir = specific;
				cur = cur->next;
			}
			specific = EMPTY;
		}
		else
			cur = cur->next;
	}
}

/**
 * @brief Associates file names with redirection tokens.
 * 
 * This function iterates through a linked list of tokens and assigns the 
 * appropriate file name to redirection operators (`<`, `>`, `<<`, `>>`). 
 * It also handles cases where redirections are ambiguous and ensures 
 * proper memory management by freeing unnecessary tokens.
 * 
 * @param cur A pointer to the first token in the list.
 */
void	put_files_for_redirections(t_token *cur, t_ms *ms)
{
	t_token	*deleted;

	while (cur)
	{
		if (is_redirect(cur->type) && cur->next && cur->next->type == WORD)
		{
			deleted = cur->next;
			cur->ambiguous = cur->next->ambiguous;
			if (!cur->ambiguous)
			{
				cur->file = ft_strdup(cur->next->data);
				if (!cur->file)
				{
					print_malloc_set_status(ms); //? can I typecast into void?
					return ;
				}
				free(deleted->data);
				if (deleted->file)
					free(deleted->file);
			}
			else
			{
				cur->file = cur->next->file;
				free(deleted->data);
			}
			cur->quote = cur->next->quote;
			cur->next = cur->next->next;
			free(deleted);
		}
		cur = cur->next;
	}
}

/*
	//printf("CUR: Type: %d, Data: %s, Quotes: %c, Redir: %d, Ambig: %d, File: %s\n", cur->type, cur->data, cur->quote, cur->specific_redir, cur->ambiguous, cur->file);
	//printf("NEXT: Type: %d, Data: %s, Quotes: %c, Redir: %d, Ambig: %d, File: %s\n", cur->next->type, cur->next->data, cur->next->quote, cur->next->specific_redir, cur->next->ambiguous, cur->next->file);
	//printf("new cur->file: %s\n", cur->file);
	//printf("new cur->file: %s\n", cur->file);
	//printf("new cur->file: %s\n", cur->file);
	//printf("LEFT: Type: %d, Data: %s, Quotes: %c, Redir: %d, Ambig: %d, File: %s\n", cur->type, cur->data, cur->quote, cur->specific_redir, cur->ambiguous, cur->file);
*/