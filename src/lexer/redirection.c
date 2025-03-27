/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 13:58:58 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 13:59:00 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

/*Если тип этого элемента - редирект - сохраняем конкретный тип
	и переходим к следующему
  Если уже встретился ранее редир и не было слова:
	если пробелы - пропускаем и переходим, сбрасываем тип
	если слова - ставим им флаг и переходим, сбрасываем тип
  Если не встретился - просто переходим, сбрасываем тип
*/

/**
 * @brief Assigns redirection flags to tokens following redirection operators.
 * 
 * This function scans a linked list of tokens and assigns a specific
 * redirection type to words that follow redirection operators 
 * (`<`, `>`, `<<`, `>>`). It ensures that the correct redirection flag 
 * is set while handling spaces and edge cases like consecutive redirections 
 * or concatenated words.
 * 
 * @param cur A pointer to the first token in the list.
 */
void	flags_for_redirections(t_token *cur)
{
	t_token_type	specific;

	specific = EMPTY;
	while (cur)
	{
		if (is_redirect(cur->type)) //this one is redir
		{
			specific = cur->type;
			cur = cur->next;
			continue ; //don't need to stay on this one in a loop
		}
		if (is_redirect(specific)) //we met redir earlier
		{
			if (cur && cur->type == SPACE)
				cur = cur->next;
			while (cur && cur->type == WORD) //case with 2 words in a row without spaces?? "a"b??
			{
				cur->specific_redir = specific;
				cur = cur->next;
			}
			specific = EMPTY; //try case with 2 redirections in a row
		}
		else //we didn't meet redir
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
void	put_files_for_redirections(t_token *cur)
{
	t_token	*deleted;

	while (cur)
	{
		if (is_redirect(cur->type) && cur->next && cur->next->type == WORD)
		{
			//printf("CUR: Type: %d, Data: %s, Quotes: %c, Redir: %d, Ambig: %d, File: %s\n", cur->type, cur->data, cur->quote, cur->specific_redir, cur->ambiguous, cur->file);
			//printf("NEXT: Type: %d, Data: %s, Quotes: %c, Redir: %d, Ambig: %d, File: %s\n", cur->next->type, cur->next->data, cur->next->quote, cur->next->specific_redir, cur->next->ambiguous, cur->next->file);
			deleted = cur->next;
			cur->ambiguous = cur->next->ambiguous;
			if (!cur->ambiguous)
			{
				cur->file = ft_strdup(cur->next->data);
				free(deleted->data);
				//printf("new cur->file: %s\n", cur->file);
				if (deleted->file)
					free(deleted->file);
			}
			else
			{
				cur->file = cur->next->file; //check it's created
				//printf("new cur->file: %s\n", cur->file);
				//printf("new cur->file: %s\n", cur->file);
				free(deleted->data);
			}
			cur->quote = cur->next->quote;
			cur->next = cur->next->next;
			free(deleted); //we have the node itself and 2 allocated fields
		} //and we free 2 of them which we don't need
		//printf("LEFT: Type: %d, Data: %s, Quotes: %c, Redir: %d, Ambig: %d, File: %s\n", cur->type, cur->data, cur->quote, cur->specific_redir, cur->ambiguous, cur->file);
		cur = cur->next;
	}
}
