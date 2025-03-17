#include "../../include/minishell.h"

/**
 * @brief Duplicates an environment entry or replaces it if the key matches.
 *
 * This function checks if the given entry matches the provided key.
 * If it matches, it returns a duplicate of the new argument (`arg`), effectively replacing the value.
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

char	*duplicate_or_replace(char *entry, char *arg, char *key, int *flag)
{
	int	len;

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
 * ensuring that the `exported` array is in lexicographical order after execution.
 *
 * @param ms The main structure containing the `exported` array to be sorted.
 *           The array is sorted in-place, modifying the `exported` field.
 */

void	sort_exported_alphaorder(t_ms *ms)
{
	int	i;
	int	j;
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
 * @brief Adds a new key to the end of the array.
 *
 * This function duplicates the given `key` and appends it to the end of the 
 * `temp` array. The `ex` = exported array is then replaced with the `temp` array, and 
 * memory for the previous `ex` array is cleaned up. The function ensures that 
 * the array is correctly terminated with a `NULL` value.
 *
 * @param ex A pointer to the array to which the new key will be added. 
 *           This array will be replaced by the `temp` array. Ex holds the exported environmental variables.
 * @param temp A pointer to the temporary array that holds the new key.
 * @param key The key to be added to the array.
 * @param i The index at which the new key should be inserted in the `temp` array.
 *          The index should be the next available spot in the array.
 */

static void	add_to_end(char ***ex, char ***temp, char *key, int i)
{
	(*temp)[i] = ft_strdup(key);
	if (!(*temp)[i])
	{
		clean_arr(temp);
		return;
	}
	i++;
	(*temp)[i] = NULL;
	clean_arr(ex);
	*ex = *temp;
}

/**
 * @brief Copies the exported environment variables into a temporary array and 
 *        adds a new key if not already present.
 *
 * This function iterates through the `ex` = exported array, checking if the given `key` 
 * already exists in any of the entries. If the `key` is found, no action is 
 * taken. If the `key` is not found, it is added to the `temp` array, which 
 * is then used to replace the `ex` array. The function ensures that the 
 * `ex` array is updated without duplicating the `key` if it already exists.
 *
 * @param key The key to be copied or added to the exported environment variables.
 * @param ex A pointer to the array holding the current exported environment variables.
 *           This array will be replaced with the `temp` array if a new key is added.
 * @param temp A pointer to the temporary array that holds a copy of the exported variables.
 * @param len The length of the `key`, used to check if it matches any existing key in the `ex` array.
 */

static void	copy_exported(char *key, char ***ex, char ***temp, int len)
{
	int	i;
	int	check;

	i = 0;
	check = 0;
	while ((*ex)[i])
	{
		if (check_env((*ex)[i], key, len, 1))
			check = 1;
		(*temp)[i] = ft_strdup((*ex)[i]);
		if (!(*temp)[i])
		{
			clean_arr(temp);
			return;
		}
		i++;
	}
	if (check == 0)
		return (add_to_end(ex, temp, key, i));
	(*temp)[i] = NULL;
	clean_arr(ex);
	*ex = *temp;
}

/**
 * @brief Adds a new key to the exported environment variables if it's valid.
 *
 * This function checks whether the `key` is valid for export. If the key is 
 * invalid, an error message is printed. If the key is valid, the function 
 * attempts to add it to the `exported` environment variable array (`ms->exported`).
 * A temporary array (`temp`) is used to hold the new environment variables, and 
 * if the key does not already exist in the array, it is added.
 *
 * @param key The key to be added to the exported environment variables.
 * @param ms A pointer to the `t_ms` structure, which contains the environment 
 *           variables (`ms->exported`) and is used for error handling.
 */

void	add_to_exported(char *key, t_ms *ms)
{
	char	**temp;
	int	len;
	
	if (check_if_valid_key(key))
		return (print_export_error(ms, key));
	len = ft_strlen(key);
	temp = allocate_temp_env(ms->exported, 2);
	if (!temp)
		return;
	copy_exported(key, &ms->exported, &temp, len);
}