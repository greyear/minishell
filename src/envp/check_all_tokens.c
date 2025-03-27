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
 * @brief Allocates and initializes a t_expand structure for variable 
 * expansion.
 * 
 * This function dynamically allocates memory for a `t_expand` structure, which 
 * is used to store the parameters needed for variable expansion. It 
 * initializes all fields to default values to ensure safe usage.
 * 
 * If memory allocation fails, an error message is printed using `perror`, and 
 * the program terminates with `exit(1)`.
 * 
 * @return A pointer to the newly allocated and initialized `t_expand` 
 *         structure.
 */
t_expand	*exp_init(t_ms *ms)
{
	t_expand	*exp;

	exp = malloc(sizeof(t_expand));
	if (!exp)
		return (print_malloc_set_status(ms));
	exp->data = NULL;
	exp->key = NULL;
	exp->len = 0;
	exp->quote = 0;
	exp->if_first = 0;
	return (exp);
}

int	expand_in_token(t_token *cur, t_ms *ms, t_bool first_in_str)
{
	char		*data_copy;
	char		*expanded;
	t_expand	*exp;

	data_copy = ft_strdup(cur->data);
	if (!data_copy)
	{
		print_malloc_set_status(ms);
		return (1);
	}
	exp = exp_init(ms);
	if (!exp)
		return (1); //?
	if (ft_strcmp(cur->data, "$") == 0 && \
			!cur->quote && cur->next && cur->next->quote)
		expanded = ft_strdup("");
	else
	{
		exp->data = cur->data;
		exp->quote = cur->quote;
		exp->if_first = first_in_str;
		expanded = handle_expansion(exp, ms);
	}
	if (!expanded)
	{
		free(data_copy);
		free(exp);
		return (1);
	}
	free(cur->data);
	cur->data = expanded;
	if (cur->specific_redir && !cur->quote && \
			data_copy[0] && !cur->data[0])
	{
		cur->ambiguous = true;
		cur->file = data_copy;
	}
	else
		free(data_copy);
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
				return (1);
		}
		cur = cur->next;
	}
	return (0);
}
