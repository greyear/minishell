#include "../../include/minishell.h"

/**
 * @brief Determines whether an environment entry should be excluded based on a variable name.
 *
 * This function checks if a given environment entry matches the specified variable name 
 * using the provided length and flag. If the entry matches the conditions for filtering 
 * (based on the environment variable name), it returns `1` to indicate that the entry 
 * should be excluded; otherwise, it returns `0`.
 *
 * @param entry The environment entry to check.
 * @param name The name of the environment variable to match against.
 * @param len The length of the environment variable name for matching.
 * @param flag If `1`, it checks for a match in the exported environment list, 
 *             and if `0`, it checks in the general environment list.
 *
 * @return `1` if the entry should be filtered (excluded), `0` otherwise.
 */

static int	should_filter_entry(char *entry, char *name, int len, int flag)
{
	if (check_env(entry, name, len, flag))
		return (1);
	return (0);
}

/**
 * @brief Copies environment entries from one list to another, excluding a specific variable.
 *
 * This function copies all entries from the source environment list (`src`) to the 
 * destination list (`dest`), except for the entry that matches the specified 
 * environment variable `name`. The function checks each entry in `src` and skips 
 * any that match the variable name. The new list is stored in `dest`.
 *
 * @param dest The destination list where the filtered environment entries will be copied.
 * @param src The source environment list to copy from.
 * @param name The name of the environment variable to exclude from the destination list.
 * @param len The length of the environment variable name for matching.
 * @param flag If `1`, the function filters entries from the exported environment list, 
 *             and if `0`, from the general environment list.
 *
 * @return `1` if the entries were successfully copied, or `0` if an error occurred.
 */

static int	copy_env_entries(char **dest, char **src, char *name, int len, int flag)
{
	int		i;
	int		x;

	i = 0;
	x = 0;
	while (src[i])
	{
		if (should_filter_entry(src[i], name, len, flag))
		{
			i++;
			continue;
		}
		dest[x] = ft_strdup(src[i]);
		if (!dest[x])
		{
			clean_arr(&dest);
			return (0);
		}
		x++;
		i++;
	}
	dest[x] = NULL;
	return (1);
}

/**
 * @brief Filters the environment entries to remove a specific variable.
 *
 * This function creates a new environment list that excludes the environment 
 * variable specified by `name`. The function checks each entry and removes 
 * the entry that matches the provided variable name. The filtered list is 
 * returned, and the original list is left unchanged.
 *
 * @param env The original environment list (`envp` or `exported`) to filter.
 * @param name The name of the environment variable to remove.
 * @param len The length of the environment variable name used for matching.
 * @param flag If `1`, the function will filter from the exported environment list, 
 *             and if `0`, from the general environment list.
 *
 * @return A new list of environment variables excluding the specified variable, 
 *         or `NULL` if an error occurs.
 */

static char	**filter_env_entries(char **env, char *name, int len, int flag)
{
	char	**filtered_env;

	filtered_env = allocate_temp_env(env, 1);
	if (!filtered_env)
		return (NULL);
	if (!copy_env_entries(filtered_env, env, name, len, flag))
		return (NULL);
	return (filtered_env);
}

/**
 * @brief Removes an environment variable from either the environment or exported 
 *        environment list.
 *
 * This function filters out the environment variable specified by `name` from 
 * the environment list (`envp`) or the exported environment list (`exported`), 
 * depending on the value of `flag`. After filtering, it updates the original 
 * list to exclude the removed variable.
 *
 * @param env A pointer to the environment list (`envp` or `exported`) that 
 *            contains the variable to be removed.
 * @param name The name of the environment variable to remove.
 * @param len The length of the environment variable name used for matching.
 * @param flag If `1`, the environment variable is removed from the exported 
 *             environment list (`exported`). If `0`, it is removed from the 
 *             general environment list (`envp`).
 */

void	rm_from_env_ex(char ***env, char *name, int len, int flag)
{
	char	**new_env;

	if (!env || !(*env))
		return;
	new_env = filter_env_entries(*env, name, len, flag);
	if (!new_env)
		return;
	clean_arr(env);
	*env = new_env;
}