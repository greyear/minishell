/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 13:59:32 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 13:59:34 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//mallocs checked

//mallocs checked

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
	if (token && token->type == WORD)
	{
		if (token->quote == 0 && (!token->data || !token->data[0]))
			return (1);
	}
	return (0);
}
