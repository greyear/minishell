/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 14:07:50 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 14:07:51 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Initializes the `envp` environment variables when no environment 
 *        variables are provided.
 *
 * This function initializes the `envp` array with environment variables for 
 * the minishell when no `envp` is passed. It allocates memory for 3 strings 
 * in the `envp` array and assigns them values:
 *   - The `PWD` variable, which is copied from `ms->exported[1]`.
 *   - The `SHLVL` variable, which is copied from `ms->exported[2]`.
 * The last element of the `envp` array is set to `NULL` to mark the end of 
 * the list. If any memory allocation fails, the function returns `0` to 
 * indicate failure.
 *
 * @param ms The minishell structure where the `envp` environment variables 
 *           will be initialized.
 * @return 1 if the `envp` is successfully initialized, 0 if memory allocation 
 *         fails.
 */
static int	initialize_envp_without_envp(t_ms *ms)
{
	ms->envp = malloc(sizeof(char *) * 4);
	if (!ms->envp)
		return (0);
	ms->envp[0] = ft_strdup(ms->exported[1]);
	if (!ms->envp[0])
		return (0);
	ms->envp[1] = ft_strdup(ms->exported[2]);
	if (!ms->envp[1])
		return (0);
	ms->envp[2] = NULL;
	return (1);
}

/**
 * @brief Initializes the exported environment variables when no environment 
 *        variables are provided.
 *
 * This function initializes the `exported` environment variables for the 
 * minishell when no `envp` is passed. It allocates memory for 4 strings in 
 * the `exported` array and assigns them values:
 *   - `OLDPWD` is initialized without a value.
 *   - `PWD` is set to the current working directory (`ms->pwd`).
 *   - `SHLVL` is set to "0", indicating the initial shell level.
 * The function ensures that each allocation is successful and that the last 
 * element in the `exported` array is `NULL`. If any memory allocation fails, 
 * the function returns `0` to indicate failure.
 *
 * @param ms The minishell structure where the exported environment variables 
 *           will be initialized.
 * @return 1 if all variables are successfully initialized, 0 if memory 
 *         allocation fails.
 */
static int	initialize_exp_without_envp(t_ms *ms)
{
	ms->exported = malloc(sizeof(char *) * 4);
	if (!ms->exported)
		return (0);
	ms->exported[0] = ft_strdup("OLDPWD");
	if (!ms->exported[0])
		return (0);
	ms->exported[1] = ft_strjoin("PWD=", ms->pwd);
	if (!ms->exported[1])
		return (0);
	ms->exported[2] = ft_strdup("SHLVL=0");
	if (!ms->exported[2])
		return (0);
	ms->exported[3] = NULL;
	return (1);
}

/**
 * @brief Initializes the minishell environment when no environment variables 
 *        are provided.
 *
 * This function is responsible for initializing the environment when no `envp` 
 * is passed to the minishell. It attempts to initialize the exported variables 
 * using `initialize_exp_without_envp`. If this fails, it sets the exit status 
 * to `MALLOC_ERR`. After that, it attempts to initialize the environment 
 * variables with `initialize_envp_without_envp`. If both initializations 
 * succeed, the function continues normally. If any memory allocation fails, 
 * the function sets the exit status to `MALLOC_ERR`, prints an error message, 
 * cleans up the minishell structure, and exits with a failure status.
 *
 * @param ms The minishell structure to be initialized.
 */
static void	initialize_without_envp(t_ms *ms)
{
	if (!initialize_exp_without_envp(ms))
		ms->exit_status = MALLOC_ERR;
	if (ms->exit_status != MALLOC_ERR)
	{
		if (!initialize_envp_without_envp(ms))
			ms->exit_status = MALLOC_ERR;
	}
	if (ms->exit_status == MALLOC_ERR)
	{
		print_malloc_set_status(ms);
		clean_struct(ms);
		exit(1);
	}
}

/**
 * @brief Creates a duplicate of a null-terminated array of strings.
 *
 * This function allocates memory for a new array of strings and 
 * copies each string from the original using `ft_strdup`.
 * If memory allocation fails at any point, it frees the allocated memory 
 * and returns NULL.
 *
 * @param original The null-terminated array of strings to be copied.
 * @return A newly allocated null-terminated copy of the original map, 
 *         or NULL if memory allocation fails.
 */
static char	**copy_array_of_strings(char **original)
{
	char	**new;
	int		i;

	i = 0;
	while (original[i])
		i++;
	new = malloc(sizeof(char *) * (i + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (original[i])
	{
		new[i] = ft_strdup(original[i]);
		if (!new[i])
		{
			clean_arr(&(new));
			return (NULL);
		}
		i++;
	}
	new[i] = NULL;
	return (new);
}

void	check_for_pwd(t_ms *ms)
{
	char	*pwd;

	add_to_exported("OLDPWD", ms);
	pwd = ft_strjoin("PWD=", ms->pwd);
	change_values_env_ex(pwd, ms);
}

/**
 * @brief Initializes the environment variables and exported variables for 
 *        the minishell.
 *
 * This function sets up the environment variables (`envp`) and exported 
 * variables (`exported`) from the provided `envp` array. If no environment 
 * variables are provided, it initializes the minishell without them. If memory 
 * allocation fails at any step, an error message is printed and the program 
 * exits with an error status.
 *
 * @param ms The minishell structure to be initialized with environment and 
 *           exported variables.
 * @param envp The array of environment variables to be copied into the 
 *             minishell structure.
 */
void	initialize_envp_and_exp(t_ms *ms, char **envp)
{
	if (!envp || !*envp)
	{
		initialize_without_envp(ms);
		ms->no_env = true;
		return ;
	}
	ms->no_env = false;
	ms->envp = copy_array_of_strings(envp);
	if (!ms->envp)
	{
		print_malloc_set_status(ms);
		clean_struct(ms);
		exit(1);
	}
	ms->exported = copy_array_of_strings(envp);
	if (!ms->exported)
	{
		print_malloc_set_status(ms);
		clean_struct(ms);
		exit(1);
	}
	check_for_pwd(ms);
}
