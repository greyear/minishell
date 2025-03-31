/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extraction.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 13:58:40 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 13:58:43 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Extracts a quoted word from the input string, handling both 
 *        single ('') and double ("") quotes.
 * 
 * This function scans the input string starting from the given index 
 * (`*start`) and extracts a word enclosed in either single or double 
 * quotes. The type of quotes is determined by the `new->quote` field.
 * 
 * @param str A pointer to the null-terminated input string.
 * @param start A pointer to the current position in `str`, updated after 
 *              processing.
 * @param new A pointer to a `t_token` struct that stores quote type and 
 *            other stuff.
 * 
 * @return A newly allocated string containing the extracted quoted word, 
 *         or NULL if memory allocation fails.
 */
char	*word_with_quotes(char *str, size_t *start, t_token *new, t_ms *ms)
{
	char	*res;
	size_t	end;

	*start = *start + 1;
	end = *start;
	while (str[end] && ((new->quote == SG_QUOT && str[end] != SG_QUOT)
			|| (new->quote == DB_QUOT && str[end] != DB_QUOT)))
		end++;
	if (str[end] == '\0')
		new->unclosed = true;
	res = (char *)ft_calloc((end - *start + 1), sizeof(char));
	if (!res)
		return ((char *)print_malloc_set_status(ms));
	ft_strlcpy(res, str + *start, end - *start + 1);
	*start = end;
	if (new->unclosed == false)
		*start = *start + 1;
	return (res);
}

/**
 * @brief Extracts a word from the input string that is not enclosed in quotes.
 * 
 * This function scans the input string starting from `*start` and extracts 
 * a word until it encounters a special character (as defined by `ft_special()`) 
 * or a whitespace character.
 * 
 * @param str A pointer to the null-terminated input string.
 * @param start A pointer to the current position in `str`, updated after 
 *              processing.
 * 
 * @return A newly allocated string containing the extracted word, or NULL 
 *         if memory allocation fails.
 */
char	*word_without_quotes(char *str, size_t *start, t_ms *ms)
{
	char	*res;
	size_t	end;

	end = *start;
	while (str[end] && !ft_special(str[end]) && !ft_isspace(str[end]))
		end++;
	res = (char *)ft_calloc((end - *start + 1), sizeof(char));
	if (!res)
		return ((char *)print_malloc_set_status(ms));
	ft_strlcpy(res, str + *start, end - *start + 1);
	*start = end;
	return (res);
}
