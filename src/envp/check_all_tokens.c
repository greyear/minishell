/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_all_tokens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 13:36:00 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 13:36:04 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Duplicates the data of a given token.
 * 
 * This function creates a copy of the `data` field from the given token 
 * structure. It uses `ft_strdup` to allocate memory and duplicate the string. 
 * If memory allocation fails, an error message is printed using 
 * `print_malloc_set_status`.
 * 
 * @param cur A pointer to the token whose data needs to be copied.
 * @param ms A pointer to the main shell structure, used for error handling.
 * 
 * @return A pointer to the newly allocated string containing the copied data, 
 *         or `NULL` if memory allocation fails.
 */
static char	*copy_token_data(t_token *cur, t_ms *ms)
{
	char	*data_copy;

	data_copy = ft_strdup(cur->data);
	if (!data_copy)
		print_malloc_set_status(ms);
	return (data_copy);
}

/**
 * @brief Expands a token's data if necessary.
 * 
 * This function processes a token's data to handle variable expansion. 
 * If the token is a standalone `$` followed by a quoted token, it expands to 
 * an empty string. Otherwise, it initializes the expansion structure and 
 * calls `handle_expansion` to perform the expansion.
 * 
 * @param exp A pointer to the expansion structure used for processing.
 * @param ms A pointer to the main shell structure for error handling.
 * @param cur A pointer to the token being processed.
 * @param first_in_str A boolean indicating if the token is the first in a 
 *                     string (affects expansion behavior).
 * 
 * @return A newly allocated string containing the expanded data, or `NULL` if 
 *         expansion fails.
 */
static char	*process_expansion(t_expand *exp, t_ms *ms, t_token *cur, \
	t_bool first_in_str)
{
	char	*expanded;

	expanded = NULL;
	if (ft_strcmp(cur->data, "$") == 0 && !cur->quote && cur->next && \
			cur->next->quote)
		expanded = ft_strdup("");
	else
	{
		exp->data = cur->data;
		exp->quote = cur->quote;
		exp->if_first = first_in_str;
		expanded = handle_expansion(exp, ms);
		cur->expanded = exp->expanded;
	}
	return (expanded);
}

/**
 * @brief Checks for ambiguous redirections and cleans up memory.
 * 
 * This function determines whether a token used in a redirection is ambiguous.
 * If the token is marked as a specific redirection, is unquoted, and either
 * expands to an empty string or multiple words, it is flagged as ambiguous.
 * Otherwise, the allocated copy of the token's data is freed.
 * 
 * @param cur A pointer to the token being processed.
 * @param data_copy A dynamically allocated copy of the token's original data.
 */
static void	check_ambiguity_and_cleanup(t_token *cur, char *data_copy)
{
	if (cur->specific_redir && !cur->quote && data_copy[0] && \
		(!cur->data[0] || has_multiple_words(cur->data)))
	{
		cur->ambiguous = true;
		cur->file = data_copy;
	}
	else
		free(data_copy);
}

/**
 * @brief Expands a token by replacing variables and handling special cases.
 * 
 * This function processes a token to expand any variables it contains, ensuring 
 * proper memory allocation and error handling. It first creates a copy of the 
 * token's data, initializes the expansion structure, and then performs the 
 * expansion. If the expansion results in an empty string or multiple words in 
 * the context of a redirection, the token is marked as ambiguous. All allocated 
 * memory is properly managed to avoid leaks.
 * 
 * @param cur A pointer to the token that needs to be expanded.
 * @param ms A pointer to the main shell structure for error handling.
 * @param first_in_str A boolean indicating if this is the first token in
 *        a string.
 * 
 * @return `0` on success, `1` if memory allocation fails at any stage.
 */
int	expand_in_token(t_token *cur, t_ms *ms, t_bool first_in_str)
{
	char		*data_copy;
	t_expand	*exp;
	char		*expanded;

	data_copy = copy_token_data(cur, ms);
	if (!data_copy)
		return (1);
	exp = initialize_expansion(ms);
	if (!exp)
	{
		free(data_copy);
		return (1);
	}
	expanded = process_expansion(exp, ms, cur, first_in_str);
	if (!expanded)
	{
		free(data_copy);
		free(exp);
		return (1);
	}
	free(cur->data);
	cur->data = expanded;
	check_ambiguity_and_cleanup(cur, data_copy);
	free(exp);
	return (0);
}

/**
 * @brief Checks a list of tokens for expandable variables and processes 
 * them.
 * 
 * This function iterates through a linked list of tokens and applies 
 * variable expansion where necessary. It skips tokens inside single 
 * quotes and those related to heredocs. The function also tracks whether 
 * the token is the first word in a line or after space, which can affect 
 * expansion behavior.
 * 
 * @param first A pointer to the first token in the list.
 * @param ms A pointer to the main shell structure, containing environment 
 * variables and shell state.
 * 
 * @return Returns `1` if an error occurs during expansion, otherwise 
 *         returns `0`.
 */
int	check_list_for_expansions(t_token *first, t_ms *ms)
{
	t_token	*cur;
	t_bool	first_in_str;

	cur = first;
	first_in_str = 1;
	while (cur)
	{
		if (cur->type == WORD && cur->quote != SG_QUOT
			&& cur->specific_redir != HEREDOC)
		{
			if (expand_in_token(cur, ms, first_in_str) == 1)
				return (1);
		}
		if (cur->type == WORD)
			first_in_str = 0;
		if (cur->type != WORD)
			first_in_str = 1;
		cur = cur->next;
	}
	return (0);
}
