/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 12:56:14 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 12:56:16 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Duplicates an environment entry or replaces it if the key matches.
 *
 * This function checks if the given entry matches the provided key.
 * If it matches, it returns a duplicate of the new argument (`arg`), 
 * effectively replacing the value.
 * Otherwise, it simply duplicates the original entry.
 *
 * @param entry The existing environment variable entry (e.g., "PATH=/usr/bin").
 * @param arg The new key-value pair to potentially replace the entry.
 * @param key The key extracted from `arg` (e.g., "PATH" from "PATH=/usr/bin").
 * @param flag Pointer to an integer indicating if the key exists. 
 *        If the key is found and replaced, `flag` is set to 2.
 * @return A newly allocated string with the updated or unchanged entry.
 *         Returns NULL if memory allocation fails.
 */
char	*dup_or_replace(char *entry, char *arg, char *key, int *flag)
{
	int		len;

	len = get_key_length(arg);
	if (check_env(entry, key, len, *flag))
	{
		*flag = 2;
		return (ft_strdup(arg));
	}
	return (ft_strdup(entry));
}

/**
 * @brief Sorts the exported environment variables in alphabetical order.
 *
 * This function sorts the `exported` array in the `ms` struct alphabetically.
 * It compares each pair of entries and swaps them if they are out of order, 
 * ensuring that the `exported` array is in lexicographical order after 
 * execution.
 *
 * @param ms The main structure containing the `exported` array to be sorted.
 *           The array is sorted in-place, modifying the `exported` field.
 */
void	sort_exported_alphaorder(t_ms *ms)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	j = 0;
	while (ms->exported[i])
	{
		j = i + 1;
		while (ms->exported[j])
		{
			if (ft_strcmp(ms->exported[i], ms->exported[j]) > 0)
			{
				temp = ms->exported[i];
				ms->exported[i] = ms->exported[j];
				ms->exported[j] = temp;
			}
			j++;
		}
		i++;
	}
}

/**
 * @brief Adds a new key to the end of the provided array of strings.
 * 
 * This function appends the string `key` to the end of the `ex` array by 
 * allocating memory for the new array (`temp`) and adding the key at the 
 * appropriate position. If memory allocation fails, the function prints 
 * an error and cleans up the allocated memory. The array `ex` is updated 
 * to point to the newly allocated array `temp`.
 * 
 * @param ex A pointer to the array that will be updated.
 * @param temp A pointer to the temporary array used to hold 
 * the updated contents.
 * @param key The string to be added to the end of the array.
 * @param i The current index where the new key will be placed.
 * 
 * @return Returns `1` on success, or `0` if an error occurred 
 * (e.g., memory allocation failure).
 */
static int	add_to_end(char ***ex, char ***temp, char *key, int i)
{
	(*temp)[i] = ft_strdup(key);
	if (!(*temp)[i])
	{
		print_malloc_error();
		clean_arr(temp);
		return (0);
	}
	i++;
	(*temp)[i] = NULL;
	clean_arr(ex);
	*ex = *temp;
	return (1);
}

/**
 * @brief Copies the contents of the `ex` array to a new array and optionally 
 *        adds a key to the end.
 * 
 * This function iterates through the `ex` array, copying each element to a new 
 * temporary array (`temp`). If the `key` is not found in `ex`, the function 
 * appends the `key` to the end of `temp`. If the `key` is already present in 
 * the `ex` array, the function simply updates `ex` to point to `temp`. In case 
 * of memory allocation failure, the function cleans up and returns `0`. 
 * 
 * @param key The key to be checked and potentially added to the `ex` array.
 * @param ex A pointer to the array that holds the exported variables.
 * @param temp A pointer to the temporary array used for copying the contents of 
 *             `ex`.
 * @param len The length of the key to check for in the `ex` array.
 * 
 * @return Returns `1` on success, or `0` if memory allocation failed or if an 
 *         error occurred.
 */
static int	copy_exported(char *key, char ***ex, char ***temp, int len)
{
	int		i;
	int		check;

	i = 0;
	check = 0;
	while ((*ex)[i])
	{
		if (check_env((*ex)[i], key, len, 1))
			check = 1;
		(*temp)[i] = ft_strdup((*ex)[i]);
		if (!(*temp)[i])
		{
			print_malloc_error();
			clean_arr(temp);
			return (0);
		}
		i++;
	}
	if (check == 0)
		return (add_to_end(ex, temp, key, i));
	(*temp)[i] = NULL;
	clean_arr(ex);
	*ex = *temp;
	return (1);
}

/**
 * @brief Adds a key to the exported variables list.
 * 
 * This function validates the `key` using `check_if_valid_key`. If the key 
 * is invalid, it prints an error and returns. If the key is valid, 
 * it allocates a temporary array to hold the exported variables, then
 * copies the contents of the `exported` list to the temporary array. 
 * If the `key` is not already in the exported list, it is added to the end.
 * In case of memory allocation failure, the function sets the `exit_status`
 * to `MALLOC_ERR`.
 * 
 * @param key The key to be added to the `exported` list.
 * @param ms A pointer to the `t_ms` structure containing the state of 
 *           the shell, including `exported` variables.
 */
void	add_to_exported(char *key, t_ms *ms)
{
	char	**temp;
	int		len;

	if (!check_if_valid_key(key))
		return (print_export_error(ms, key));
	len = ft_strlen(key);
	temp = allocate_temp_env(ms->exported, 2);
	if (!temp)
		return ;
	if (!copy_exported(key, &ms->exported, &temp, len))
		ms->exit_status = MALLOC_ERR;
}
