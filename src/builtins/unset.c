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
			print_malloc_error();
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
 * @brief Removes an entry from the environment or exported variables.
 * 
 * This function removes the specified `name` (environment variable or exported variable) from either the
 * `env` or `exported` variables list, depending on the `flag`. The `env` array is first checked to ensure
 * it is not NULL. A temporary array (`new_env`) is allocated to hold the remaining entries after removing
 * the target variable. If the allocation or copying fails, the function returns `0`. Otherwise, it cleans up
 * the old environment or exported array and updates it with the new one, returning `1` on success.
 * 
 * @param env A pointer to the environment or exported variables list (`char **`).
 * @param name The name of the environment or exported variable to remove.
 * @param len The length of the `name` to assist in locating the correct entry.
 * @param flag A flag to determine which list to modify (e.g., `env` or `exported`).
 * 
 * @return Returns `1` on success, `0` on failure.
 */

int	rm_from_env_ex(char ***env, char *name, int len, int flag)
{
	char **new_env;

	if (!env || !(*env))
		return (1);
	new_env = allocate_temp_env(*env, 1);
	if (!new_env || !copy_env_entries(new_env, *env, name, len, flag))
		return (0);
	clean_arr(env);
	*env = new_env;
	return (1);
}
/**
 * @brief Processes each entry in the "unset" command and removes the corresponding
 *        environment variable from both the exported and shell environments.
 *
 * This function is called for each argument passed to the "unset" command. It 
 * extracts the environment variable name from the argument, checks if it's a valid 
 * key, and if valid, removes it from both the `exported` and `envp` lists. If the 
 * key is invalid, the function simply returns without making any changes.
 *
 * @param args The array of command arguments, with the environment variable to unset 
 *             located at the current index `i`.
 * @param i The index of the argument in `args` that specifies the environment 
 *          variable to unset.
 * @param ms The shell structure containing execution state information, including 
 *           the `exported` and `envp` environment lists.
 * @param len The length of the environment variable key to extract from the argument.
 */

static void	process_unset_entry(char **args, int i, t_ms *ms, int len)
{
	char	*name;

	name = extract_key(args[i], len);
	if (!name)
	{
		ms->exit_status = MALLOC_ERR;
		return;
	}
	if (!check_if_valid_key(name))
	{
		free(name);
		return;
	}
	if (!rm_from_env_ex(&ms->exported, name, len, 1) 
		|| !rm_from_env_ex(&ms->envp, name, len, 0))
		ms->exit_status = MALLOC_ERR;
	free(name);
}

/**
 * @brief Handles the "unset" command, which removes environment variables.
 *
 * This function processes the given arguments and attempts to remove the 
 * corresponding environment variables from both the exported and shell environments. 
 * If the argument is not a valid environment variable key, it skips the removal.
 * The exit status is updated based on the success or failure of the unset operation.
 *
 * @param args The array of command arguments.
 * @param ms The shell structure containing execution state information, including 
 *           the exit status.
 */

void	handle_unset(char **args, t_ms *ms)
{
	int		i;
	int		len;

	i = 1;
	ms->exit_status = 0;
	if (!args[1])
		return;
	if (args[1][0] && args[1][0] == '-')
	{
		print_flag_error(args);
		ms->exit_status = 2;
		return;
	}
	while (args[i])
	{
		len = get_key_length(args[i]);
		if (len != 0 || !(ft_strchr(args[i], '='))
			|| args[i][0] != '=')
			process_unset_entry(args, i, ms, len);
		if (ms->exit_status == MALLOC_ERR)
			break;
		i++;
	}
}