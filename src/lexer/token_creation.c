/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_creation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 13:59:24 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 13:59:27 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//mallocs checked

#include "../../include/minishell.h"

/**
 * @brief Determines the type of a token based on a given character in a string.
 * 
 * This function checks a character (and sometimes the next character) to 
 * classify it as a specific token type. It distinguishes between different 
 * types of redirections, pipes, spaces, and words.
 * 
 * @param str The input string containing the character to be analyzed.
 * @param i The index of the character in the string to classify.
 * 
 * @return The corresponding token type (`HEREDOC`, `IN`, `APPEND`, `OUT`, 
 *         `PIPE`, `SPACE`, or `WORD`).
 */
t_token_type	define_token_type(char *str, size_t i)
{
	//&?
	if (str[i] == '<' && str[i + 1] == '<')
		return (HEREDOC);
	else if (str[i] == '<')
		return (IN);
	else if (str[i] == '>' && str[i + 1] == '>')
		return (APPEND);
	else if (str[i] == '>')
		return (OUT);
	else if (str[i] == '|' && str[i + 1] != '|')
		return (PIPE);
	else if (ft_isspace(str[i]))
		return (SPACE);
	else
		return (WORD);
}

/**
 * @brief Initializes a t_token structure with default values.
 * 
 * This function sets all fields of the given token to their default states, 
 * ensuring proper initialization before further use. It prevents 
 * uninitialized memory issues and ensures predictable behavior.
 * 
 * @param new A pointer to the t_token structure to be initialized.
 */
static void	default_token_values(t_token *new)
{
	new->data = NULL;
	new->file = NULL;
	new->next = NULL;
	new->quote = 0;
	new->unclosed = false;
	new->specific_redir = EMPTY;
	new->ambiguous = false;
}

/**
 * @brief Processes a word token, handling quoted and unquoted words.
 * 
 * This function determines whether the token starts with a single (`'`) or 
 * double (`"`) quote and processes it accordingly. If the word is quoted, 
 * it extracts the content while preserving the quote type. Otherwise, it 
 * extracts a regular word without quotes.
 * 
 * @param str The input string containing the token to process.
 * @param i A pointer to the current index in the string, which will be 
 *          updated as characters are processed.
 * @param new A pointer to the newly created token structure where the 
 *            processed word data will be stored.
 */
static void	process_word_token(char *str, size_t *i, t_token *new, t_ms *ms)
{
	if (str[*i] == SG_QUOT || str[*i] == DB_QUOT)
	{
		new->quote = str[*i];
		new->data = word_with_quotes(str, i, new, ms);
	}
	else
		new->data = word_without_quotes(str, i, ms);
}

/**
 * @brief Creates a new token from the input string.
 * 
 * This function allocates memory for a new token and initializes its values. 
 * It determines how to process the token based on its type: words are processed 
 * separately to handle quotes, spaces are skipped, and special tokens are 
 * handled accordingly. If memory allocation fails, the function sets an error 
 * status and returns NULL.
 * 
 * @param str The input string from which the token is extracted.
 * @param i A pointer to the current index in the string, which will be updated 
 *          as characters are processed.
 * @param type The type of token to be created.
 * @param ms A pointer to the main minishell structure, used for error handling.
 * 
 * @return A pointer to the newly created token, or NULL if an error occurs.
 */
t_token	*create_new_token(char *str, size_t *i, t_token_type type, t_ms *ms)
{
	t_token	*new;

	new = (t_token *)ft_calloc(1, sizeof(t_token));
	if (!new)
	{
		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
		return (NULL);
	}
	default_token_values(new);
	new->type = type;
	if (type == WORD)
	{
		process_word_token(str, i, new, ms);
		if (!new->data)
			return (NULL);
	}
	else if (type == SPACE)
		skip_whitespaces(str, i);
	else
		skip_special_tokens(str, i, type);
	return (new);
}
