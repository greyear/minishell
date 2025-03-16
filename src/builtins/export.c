#include "../../include/minishell.h"

/**
 * @brief Creates a temporary copy of the environment array, updating or preserving entries.
 *
 * This function allocates a new environment array with extra space for potential additions.
 * It copies existing variables while replacing the value of the given key if necessary.
 * If memory allocation fails, the function cleans up and returns NULL.
 *
 * @param arg The full "KEY=VALUE" string to be added or updated.
 * @param env A pointer to the environment/exported variable array.
 * @param key The key extracted from the argument to identify the variable.
 * @param flag A pointer indicating whether the key exists (1 if updated, 0 if new).
 *
 * @return A newly allocated environment array with the updated values or NULL on failure.
 */

static char	**copy_to_temp(char *arg, char ***env, char *key, int *flag)
{
	char	**temp;
	int	i;

	i = 0;
	temp = allocate_temp_env(*env, 2);
	if (!temp)
		return (NULL);
	while ((*env)[i])
	{
		temp[i] = duplicate_or_replace((*env)[i], arg, key, flag);
		if (!temp[i])
		{
			clean_arr(&temp);
			return (NULL);
		}
		i++;
	}
	temp[i] = NULL;
	return (temp);
}

/**
 * @brief Updates or adds a variable to the environment or exported list.
 *
 * This function searches for an existing variable matching the given key in the specified
 * environment array. If the key is found, it updates the value; otherwise, it adds a new entry.
 * If `flag == 2`, no new entry is added. The function ensures proper memory management.
 *
 * @param arg The full "KEY=VALUE" string to be added or updated.
 * @param env A pointer to the environment/exported variable array.
 * @param key The key extracted from the argument to identify the variable.
 * @param flag A flag indicating whether to update (if key exists) or add a new entry.
 */

static void	change_values(char *arg, char ***env, char *key, int flag)
{
	char	**temp;
	int		i;

	i = 0;
	temp = copy_to_temp(arg, env, key, &flag);
	if (!temp || !*temp)
		return;
	if (flag != 2)
	{
		while (temp[i])
			i++;
		temp[i] = ft_strdup(arg);
		if (!temp[i])
		{
			print_error(ERR_MALLOC);
			clean_arr(&temp);
			return;
		}
		i++;
		temp[i] = NULL;
	}
	clean_arr(env);
	*env = temp;
}

/**
 * @brief Updates or adds an environment variable in both exported and environment lists.
 *
 * This function extracts the key from the given argument and checks if it is valid.
 * If the key is invalid, an error message is printed, and the function returns.
 * Otherwise, it updates or adds the variable in both the exported list and the environment list.
 *
 * @param arg The argument containing the environment variable in the format "KEY=VALUE".
 * @param ms A pointer to the shell's main structure containing environment variables and state.
 */

static void	change_values_env_ex(char *arg, t_ms *ms)
{
	int	len;
	char	*key;

	len = get_key_length(arg);
	key = extract_key(arg, len);
	if (!key)
	{
		ms->exit_status = 1;
		return;
	}
	if (check_if_valid_key(key) == 1)
	{
		free(key);
		print_export_error(ms, arg);
		return;
	}
	change_values(arg, &ms->exported, key, 1);
	change_values(arg, &ms->envp, key, 0);
	free(key);
}

/**
 * @brief Processes arguments for the `export` command.
 *
 * This function iterates through the arguments provided to the `export` command
 * and determines whether each argument represents an environment variable assignment
 * (contains `=`) or a variable that should be marked for export without a value.
 *
 * - If the argument contains `=`, it updates or adds the variable to the environment.
 * - Otherwise, it adds the variable to the list of exported names without assigning a value.
 * - After each modification, the exported variables are sorted alphabetically.
 *
 * @param args A null-terminated array of arguments passed to `export`.
 * @param ms A pointer to the shell's main structure containing environment variables and state.
 */

static void	process_arguments(char **args, t_ms *ms)
{
	int	i;

	i = 1;
	if (args[1][0] && args[1][0] == '-')
	{
		print_flag_error(args);
		ms->exit_status = 2;
		return;
	}
	while (args[i])
	{
		if (ft_strchr(args[i], '='))
			change_values_env_ex(args[i], ms);
		else
			add_to_exported(args[i], ms);
		if (ms->exit_status == 1)
			return;
		sort_exported_alphaorder(ms);
		i++;
	}
}

/**
 * @brief Handles the `export` built-in command in the shell.
 *
 * This function processes the `export` command, which is used to add or modify 
 * environment variables. It follows these steps:
 * 
 * 1. If `ms` or `args` are NULL, or if the command is not "export", it returns immediately.
 * 2. If only "export" is provided with no arguments, it sorts and prints the exported variables.
 * 3. If arguments are provided, it processes each argument to update the environment variables.
 *
 * @param args A null-terminated array of strings representing the command and its arguments.
 * @param ms A pointer to the shell's main structure containing environment variables and state.
 */

void	handle_export(char **args, t_ms *ms)
{
	int	arg_count;

	arg_count = 0;
	if (!args || !*args)
		return;
	if (ft_strcmp(args[0], "export") != 0)
		return;
	ms->exit_status = 0;
	while (args[arg_count])
		arg_count++;
	if (arg_count == 1)
	{
		sort_exported_alphaorder(ms);
		print_exported(ms);
		return;
	}
	process_arguments(args, ms);
}