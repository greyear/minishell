/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 12:56:29 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 12:56:30 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Checks if an environment variable matches a given key.
 *
 * This function checks whether the environment variable `env` starts with the 
 * string `key` and whether it matches specific conditions based on the `flag`. 
 * The `flag` determines whether the function checks for an equal sign (`=`) 
 * after the key, or if it allows for an empty string after the key.
 *
 * @param env  The environment variable to check.
 * @param key  The key to match in the environment variable.
 * @param len  The length of the key.
 * @param flag A flag that modifies the behavior of the function:
 *             - `flag == 0`: checks if the environment variable starts with the 
 *               key followed by `=`.
 *               Used for checking environmental variables.
 *             - `flag == 1`: checks if the environment variable starts with the 
 *               key followed by `=` or ends right after the key.
 *               Used for checking exported environmental variables.
 * @return 1 if the environment variable matches the conditions, otherwise 0.
 */
int	check_env(char *env, char *key, int len, int flag)
{
	if (ft_strncmp(key, env, len) != 0)
		return (0);
	if (flag == 0 && (env[len] && env[len] == '='))
		return (1);
	if (flag == 1 && (env[len] == '\0'
			|| (env[len] && env[len] == '=')))
		return (1);
	return (0);
}

/**
 * @brief Retrieves the value of an environment variable from the environment.
 *
 * This function searches through the array of environment variables `envp` to 
 * find a variable whose key matches the provided `key` and returns its 
 * associated value. The key is expected to be followed by an equal sign (`=`) 
 * in the environment variable string.
 *
 * @param key   The key of the environment variable whose value is to be 
 *              retrieved.
 * @param envp  A pointer to the environment variables array (an array 
 *              of strings, each in the format "KEY=VALUE").
 *
 * @return The value of the environment variable if found, or NULL if the key is 
 *         not present in the environment.
 */
char	*get_env_value(char *key, char **envp)
{
	int		i;
	size_t	len;

	if (!envp)
		return (NULL);
	i = 0;
	len = ft_strlen(key);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, len) == 0
			&& envp[i][len] && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

/**
 * @brief Allocates memory for a temporary environment variable array.
 *
 * This function allocates memory for a new array of environment variables. 
 * The size of the array is determined by the current number of environment 
 * variables (`env`), plus an additional `x` entries to account for future 
 * additions.
 *
 * @param env  A pointer to the current environment variable array.
 * @param x    The number of additional entries to allocate space for.
 *
 * @return A pointer to the newly allocated temporary environment variable 
 *         array, or NULL if memory allocation fails or the original environment 
 *         array is empty.
 */
char	**allocate_temp_env(char **env, int x)
{
	char	**temp;
	int		i;

	i = 0;
	if (!env || !*env)
		return (NULL);
	while (env[i])
		i++;
	temp = malloc(sizeof(char *) * (i + x));
	if (!temp)
		return (NULL);
	return (temp);
}

/**
 * @brief Prints an array of strings to the standard output.
 *
 * This function iterates through a null-terminated array of strings (`a`)
 * and prints each string followed by a newline (`\n`).
 *
 * @param a  A null-terminated array of strings to be printed.
 */
void	print_array(char **a)
{
	int		i;

	i = 0;
	while (a[i])
	{
		ft_putstr_fd(a[i], STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		i++;
	}
}

/**
 * @brief Creates arguments for exporting the `OLDPWD` environment variable.
 * 
 * This function dynamically allocates memory for an array of strings to store 
 * the `export` command and the `OLDPWD` environment variable with its value 
 * set to the previous working directory (`pwd_before`). If any memory 
 * allocation fails, the function sets the exit status to `MALLOC_ERR` and 
 * prints an error message. The generated arguments are returned in the `args` 
 * parameter.
 * 
 * @param args A pointer to a pointer to an array of strings, which will 
 *             store the `export` command and the `OLDPWD` variable.
 * @param ms A pointer to the `t_ms` structure, which manages shell-related 
 *           data, including exit status.
 * @param pwd_before A string representing the previous working directory.
 * 
 * @return 1 on success, 0 on failure (due to memory allocation errors).
 */
int	make_cd_args(char ***args, t_ms *ms, char *pwd_before)
{
	(*args) = malloc(sizeof(char *) * 3);
	if (!(*args))
	{
		print_malloc_set_status(ms);
		return (0);
	}
	(*args)[0] = ft_strdup("export");
	if (!(*args)[0])
		return (0);
	(*args)[1] = ft_strjoin("OLDPWD=", pwd_before);
	if (!(*args)[1])
		return (0);
	(*args)[2] = NULL;
	return (1);
}
