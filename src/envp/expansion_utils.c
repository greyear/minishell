/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 13:36:08 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 13:36:10 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Appends a dynamically allocated string to the result string.
 * 
 * This function concatenates `new_part` to `*result`, creating a new 
 * dynamically allocated string. The original `*result` is freed after 
 * concatenation. If memory allocation fails, an error message is printed, 
 * the shell's exit status is set to `MALLOC_ERR`, and `new_part` is freed.
 * 
 * @param result A pointer to the dynamically allocated result string. It is 
 *               updated with the newly concatenated string.
 * @param new_part A dynamically allocated string to be appended to `*result`. 
 *                 It is freed after use.
 * @param ms A pointer to the `t_ms` structure, which manages shell-related
 *           data, including exit status.
 * 
 * @return None. The function modifies `*result` and updates `ms->exit_status` 
 *         on failure.
 */
static void	append_to_result(char **result, char *new_part, t_ms *ms)
{
	char	*temp;

	if (!new_part)
		return ;
	temp = ft_strjoin(*result, new_part);
	if (!temp)
	{
		print_malloc_set_status(ms);
		free(new_part);
		return ;
	}
	free(*result);
	*result = temp;
	free(new_part);
}

/**
 * @brief Retrieves the value of an environment variable.
 * 
 * This function searches for an environment variable in the given `envp` array
 * by matching the key stored in `exp->key`. If found, it returns a newly 
 * allocated string containing the variable's value. If `exp->quote` is 0, 
 * extra spaces in the value are removed using `handle_spaces`. If the key is 
 * not found, an empty string is returned.
 * 
 * @param envp The array of environment variables.
 * @param exp A pointer to the `t_expand` structure containing the key to
 *            search for.
 * 
 * @return A dynamically allocated string containing the environment variable's 
 *         value. Returns an empty string if the key is not found or `NULL` if 
 *         memory allocation fails.
 */
static char	*find_env_value(char **envp, t_expand *exp)
{
	int		i;
	char	*copy;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], exp->key, exp->len) == 0
			&& envp[i][exp->len] && envp[i][exp->len] == '=')
		{
			copy = ft_strdup(envp[i] + exp->len + 1);
			if (!copy)
				return (NULL);
			if (exp->quote == 0) //for heredocs????
				return (handle_spaces(copy, exp));
			else
				return (copy);
		}
		i++;
	}
	return (ft_strdup(""));
}

/**
 * @brief Expands a shell variable and appends its value to the result string.
 *
 * This function expands a shell variable based on its key. If the key is "?",
 * it appends the exit status. If the key is a number, it appends an empty 
 * string. For other keys, it looks up the value in the environment variables.
 * The  expanded value is then appended to the `result` string. If memory 
 * allocation fails during the process, it prints an error and updates the 
 * exit status to indicate a failure.
 *
 * @param ms The shell structure containing the current state, including the 
 *           exit status and environment variables.
 * @param exp The structure containing the key of the variable to expand and 
 *            the result to append to.
 * @param result A pointer to the string where the expanded variable value will 
 *               be appended.
 */
void	expand_variable(t_ms *ms, t_expand *exp, char **result)
{
	char	*expanded;

	if (!exp->key || !*(exp->key))
		return ;
	if (exp->key[0] == '?')
		expanded = ft_itoa(ms->exit_status);
	else if (ft_isdigit(exp->key[0]))
		expanded = ft_strdup("");
	else
		expanded = find_env_value(ms->envp, exp);
	if (!expanded)
	{
		print_malloc_set_status(ms);
		return ;
	}
	if (!expanded)
	{
		print_malloc_set_status(ms);
		return ;
	}
	append_to_result(result, expanded, ms);
}
