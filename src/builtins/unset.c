/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 12:56:55 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 12:56:57 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Copies environment variable entries from the source to the destination 
 *        array, excluding entries that match a specified key.
 *
 * This function iterates over the `src` array of environment variables and 
 * copies each entry that doesn't match the provided `key` (as determined by 
 * the `check_env` function) to the `dest` array. If memory allocation fails 
 * during copying, the function cleans up the destination array and returns 0. 
 * The destination array is null-terminated after all valid entries are copied.
 *
 * @param dest A pointer to the destination array where valid environment 
 *             variable entries will be copied.
 * @param src The source array of environment variable entries to process.
 * @param key The environment variable key to filter out.
 * @param flag A flag that influences the filtering behavior of `check_env`.
 * 
 * @return 1 if the copying is successful, 0 if an error occurs during memory 
 *         allocation.
 */
static int	copy_env_entries(char ***dest, char **src, char *key, int flag)
{
	int		i;
	int		x;
	int		len;

	i = 0;
	x = 0;
	len = get_key_length(key);
	while (src[i])
	{
		if (check_env(src[i], key, len, flag))
		{
			i++;
			continue ;
		}
		(*dest)[x] = ft_strdup(src[i]);
		if (!(*dest)[x])
		{
			clean_arr(dest);
			return (0);
		}
		x++;
		i++;
	}
	(*dest)[x] = NULL;
	return (1);
}

/**
 * @brief Removes a specified environment variable from the environment list.
 *
 * This function creates a new environment array and copies all entries from 
 * the original environment array, excluding the entry that matches the 
 * specified `key`. If memory allocation for the new environment array fails 
 * or copying the entries fails, the function returns 0. If successful, the 
 * original environment array is replaced with the new one, and the old 
 * environment array is cleaned up.
 *
 * @param env A pointer to the environment array, which will be modified by 
 *            removing the specified variable.
 * @param key The key of the environment variable to remove.
 * @param flag A flag that influences the filtering behavior of `check_env`.
 * 
 * @return 1 if the removal and replacement are successful, 0 if an error occurs.
 */
static int	rm_from_env_ex(char ***env, char *key, int flag)
{
	char	**new_env;

	if (!env || !(*env))
		return (1);
	new_env = allocate_temp_env(*env, 1);
	if (!new_env)
		return (0);
	if (!copy_env_entries(&new_env, *env, key, flag))
		return (0);
	clean_arr(env);
	*env = new_env;
	return (1);
}

/**
 * @brief Processes a single "unset" command entry by removing the corresponding 
 *        environment variable from both the exported and shell environments.
 *
 * This function extracts the key from the given argument, checks if it's a 
 * valid environment variable name, and attempts to remove the variable from 
 * both the `exported` and `envp` environment arrays. If memory allocation 
 * fails during the removal process, it prints an error and updates the exit 
 * status to indicate failure. The key is freed after processing.
 *
 * @param args The array of command arguments, with the environment variable 
 *             to unset located at the current index `i`.
 * @param i The index of the argument in `args` that specifies the environment 
 *          variable to unset.
 * @param ms The shell structure containing execution state information, 
 *           including the `exported` and `envp` environment lists.
 * @param len The length of the environment variable key to extract from 
 *            the argument.
 */
static void	process_unset_entry(char **args, int i, t_ms *ms, int len)
{
	char	*key;

	key = extract_key(args[i], len);
	if (!key)
	{
		print_malloc_set_status(ms);
		return ;
	}
	if (!check_if_valid_key(key))
	{
		free(key);
		return ;
	}
	if (!rm_from_env_ex(&ms->exported, key, 1)
		|| !rm_from_env_ex(&ms->envp, key, 0))
	{
		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
	}
	free(key);
}

/**
 * @brief Handles the "unset" command, which removes environment variables.
 *
 * This function processes the given arguments and attempts to remove the 
 * corresponding environment variables from both the exported and shell 
 * environments. If the argument is not a valid environment variable key, it 
 * skips the removal. The exit status is updated based on the success or 
 * failure of the unset operation.
 *
 * @param args The array of command arguments.
 * @param ms The shell structure containing execution state information, 
 *           including the exit status.
 */
void	handle_unset(char **args, t_ms *ms)
{
	int		i;
	int		len;

	i = 1;
	ms->exit_status = 0;
	if (!args[1])
		return ;
	if (args[1][0] && args[1][0] == '-')
	{
		print_flag_error(args);
		ms->exit_status = 2;
		return ;
	}
	while (args[i])
	{
		len = get_key_length(args[i]);
		if (len != 0 || !(ft_strchr(args[i], '='))
			|| args[i][0] != '=')
			process_unset_entry(args, i, ms, len);
		if (ms->exit_status == MALLOC_ERR)
			break ;
		i++;
	}
}
