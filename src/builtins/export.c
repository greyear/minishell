/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 12:56:06 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 12:56:08 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Creates a temporary copy of the environment array, updating or 
 *        preserving entries.
 *
 * This function allocates a new environment array with extra space for 
 * potential additions. It copies existing variables while replacing the 
 * value of the given key if necessary. If memory allocation fails, the 
 * function cleans up and returns NULL.
 *
 * @param arg The full "KEY=VALUE" string to be added or updated.
 * @param env A pointer to the environment/exported variable array.
 * @param key The key extracted from the argument to identify the variable.
 * @param flag A pointer indicating whether the key exists (1 if updated, 
 *             0 if new).
 *
 * @return A newly allocated environment array with the updated values or 
 *         NULL on failure.
 */
static char	**copy_to_temp(char *arg, char ***env, char *key, int *flag)
{
	char	**temp;
	int		i;

	i = 0;
	temp = allocate_temp_env(*env, 2);
	if (!temp)
		return (NULL);
	while ((*env)[i])
	{
		temp[i] = dup_or_replace((*env)[i], arg, key, flag);
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
 * @brief Changes the value of an environment variable in the specified 
 *        environment array.
 * 
 * This function updates the environment variable specified by `key` with 
 * the value provided in `arg`. If the flag is not 2, it appends the new 
 * value to the environment array. The environment array (`env`) is 
 * dynamically resized to accommodate the new value. The function handles 
 * memory allocation for the new environment array and frees the old one. 
 * If any memory allocation fails, the function cleans up and returns `0`, 
 * signaling an error.
 * 
 * @param arg The new environment variable value (in the form of a string).
 * @param env A pointer to the environment array (`env`), which will be 
 *            updated.
 * @param key The key of the environment variable to be modified.
 * @param flag A flag indicating whether the value should be added (1), or 
 *             replaced (2). If the flag is not 2, the value is appended.
 * 
 * @return Returns `1` on success, or `0` if an error occurred (such as 
 *         memory allocation failure).
 */
static int	change_values(char *arg, char ***env, char *key, int flag)
{
	char	**temp;
	int		i;

	i = 0;
	temp = copy_to_temp(arg, env, key, &flag);
	if (!temp || !*temp)
		return (0);
	if (flag != 2)
	{
		while (temp[i])
			i++;
		temp[i] = ft_strdup(arg);
		if (!temp[i])
		{
			clean_arr(&temp);
			return (0);
		}
		i++;
		temp[i] = NULL;
	}
	clean_arr(env);
	*env = temp;
	return (1);
}

/**
 * @brief Changes environment variable values in both `exported` 
 * and `envp` arrays.
 * 
 * This function extracts the key from the given argument, checks if the key is 
 * valid, and updates the corresponding environment variable in both `exported` 
 * and `envp` arrays using the `change_values` function. If the key is invalid, 
 * an error is printed, and the function returns without modifying 
 * the environment. If any memory allocation fails, the shell's exit status is 
 * set to `MALLOC_ERR`, and the function ensures that memory is freed before
 * returning.
 * 
 * @param arg The argument containing the environment variable to modify.
 * @param ms A pointer to the `t_ms` structure, which contains environment 
 *           variables and shell-related data, including the exit status.
 * 
 * @return None. Updates the environment variables directly.
 */
static void	change_values_env_ex(char *arg, t_ms *ms)
{
	int		len;
	char	*key;

	len = get_key_length(arg);
	key = extract_key(arg, len);
	if (!key)
	{
		print_malloc_set_status(ms);
		return ;
	}
	if (!check_if_valid_key(key))
	{
		free(key);
		print_export_error(ms, arg);
		return ;
	}
	if (!change_values(arg, &ms->exported, key, 1)
		|| !change_values(arg, &ms->envp, key, 0))
	{
		print_malloc_set_status(ms);
		free(key);
		return ;
	}
	free(key);
}

/**
 * @brief Processes arguments for the `export` command.
 *
 * This function iterates through the arguments provided to the `export` command
 * and determines whether each argument represents an environment variable 
 * assignment (contains `=`) or a variable that should be marked for export 
 * without a value.
 *
 * - If the argument contains `=`, it updates or adds the variable to the 
 *   environment.
 * - Otherwise, it adds the variable to the list of exported names without 
 *   assigning a value.
 * - After each modification, the exported variables are sorted alphabetically.
 *
 * @param args A null-terminated array of arguments passed to `export`.
 * @param ms A pointer to the shell's main structure containing environment 
 *           variables and state.
 */
static void	process_arguments(char **args, t_ms *ms)
{
	int		i;

	i = 1;
	if (args[1][0] && args[1][0] == '-')
	{
		print_flag_error(args);
		ms->exit_status = 2;
		return ;
	}
	while (args[i])
	{
		if (ft_strchr(args[i], '='))
			change_values_env_ex(args[i], ms);
		else
			add_to_exported(args[i], ms);
		if (ms->exit_status == MALLOC_ERR)
			return ;
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
 * 1. If `ms` or `args` are NULL, or if the command is not "export", it returns 
 * immediately.
 * 2. If only "export" is provided with no arguments, it sorts and prints the 
 * exported variables.
 * 3. If arguments are provided, it processes each argument to update the 
 * environment variables.
 *
 * @param args A null-terminated array of strings representing the command and 
 *             its arguments.
 * @param ms A pointer to the shell's main structure containing environment 
 *           variables and state.
 */
void	handle_export(char **args, t_ms *ms)
{
	int		arg_count;

	arg_count = 0;
	ms->exit_status = 0;
	while (args[arg_count])
		arg_count++;
	if (arg_count == 1)
	{
		sort_exported_alphaorder(ms);
		print_exported(ms);
		return ;
	}
	process_arguments(args, ms);
}
