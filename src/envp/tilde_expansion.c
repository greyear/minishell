/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 16:59:52 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/31 16:59:55 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Expands the tilde (`~`) in a token to the user's home directory.
 * 
 * This function checks if the given token starts with a tilde (`~`). If 
 * it does, it replaces it with the user's home directory path. The function 
 * handles the following cases:
 * - `~` alone is replaced with the home directory.
 * - `~/something` expands to `<home>/something`.
 * - If `~` is part of another word (e.g., `hello~`), no expansion occurs.
 * 
 * @param cur A pointer to the token containing the potential tilde.
 * @param ms A pointer to the main shell structure, used to retrieve the 
 * home directory.
 * 
 * @return Returns `0` if expansion is successful or not needed, and `1` 
 * if a memory allocation error occurs.
 */
int	expand_tilde(t_token *cur, t_ms *ms)
{
	char	*home;
	char	*new_data;

	if (!cur->data || cur->data[0] != '~')
		return (0);
	home = get_home_directory(ms, 1);
	if (!home)
		return (0);
	if (cur->data[1] == '\0')
		new_data = ft_strdup(home);
	else if (cur->data[1] == '/')
		new_data = ft_strjoin(home, &cur->data[1]);
	else
	{
		free(home);
		return (0);
	}
	free(home);
	if (!new_data)
		return (1);
	free(cur->data);
	cur->data = new_data;
	return (0);
}

/**
 * @brief Scans a list of tokens and expands tilde (`~`) where applicable.
 * 
 * This function iterates through a linked list of tokens and checks for 
 * the tilde (`~`) character at the beginning of each token's data. If the 
 * token is not enclosed in single (`'`) or double (`"`) quotes, the tilde 
 * is expanded to the user's home directory.
 * 
 * @param first A pointer to the first token in the list.
 * @param ms A pointer to the main shell structure, used to retrieve the home 
 *           directory.
 * 
 * @return Returns `0` if successful, or `1` if an expansion fails due to memory 
 *         allocation errors.
 */
int	check_list_for_tilde(t_token *first, t_ms *ms)
{
	t_token	*cur;

	cur = first;
	while (cur)
	{
		if (cur->type == WORD && cur->quote != SG_QUOT && cur->quote != DB_QUOT
			&& cur->data[0] == '~')
		{
			if (expand_tilde(cur, ms) == 1)
			{
				print_malloc_set_status(ms);
				return (1);
			}
		}
		cur = cur->next;
	}
	return (0);
}
