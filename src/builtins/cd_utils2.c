/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 12:55:13 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 12:55:15 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Handles an empty OLDPWD variable.
 *
 * If OLDPWD is empty, this function updates it with the current PWD.
 * If PWD is also empty, it retrieves the current working directory 
 * using getcwd() and updates PWD instead.
 * 
 * - Retrieves PWD from the environment.
 * - If PWD is empty, calls getcwd() and updates PWD.
 * - Otherwise, updates OLDPWD with the current PWD value.
 * - If getcwd() fails, prints an error and sets exit_status to 1.
 * 
 * @param ms A pointer to the main shell structure containing 
 *           environment variables.
 */
static void	handle_empty_oldpwd(t_ms *ms)
{
	char	*current_pwd;
	char	cwd[1024];

	ft_putstr_fd("\n", STDOUT_FILENO);
	current_pwd = get_env_value("PWD", ms->envp);
	if (!current_pwd || *current_pwd == '\0')
	{
		if (getcwd(cwd, sizeof(cwd)) == NULL)
		{
			perror("getcwd failed");
			ms->exit_status = 1;
			return ;
		}
		update_env_var(ms, "PWD=", cwd);
		return ;
	}
	update_env_var(ms, "OLDPWD=", current_pwd);
	return ;
}

/**
 * @brief Retrieves the previous working directory stored in the `OLDPWD` 
 * environment variable.
 * 
 * This function attempts to fetch the value of the `OLDPWD` environment 
 * variable and perform several checks:
 * - If `OLDPWD` is not set, it prints an error message and returns `NULL`.
 * - If `OLDPWD` is empty, it calls `handle_empty_oldpwd` to handle this case
 *   and returns `NULL`.
 * - If `OLDPWD` points to a non-existent file or directory, it prints an 
 *   error and returns `NULL`.
 * - If `OLDPWD` is valid, it prints the directory and returns a duplicate 
 *   of the path.
 * 
 * The function ensures that the `OLDPWD` value is valid and points to an 
 * existing directory, which is required for operations like changing back 
 * to the previous directory using `cd -`.
 * 
 * @param ms The main shell structure, containing the environment variables 
 *           `envp` and exit status.
 * 
 * @return Returns a newly allocated string containing the `OLDPWD` 
 *         directory, or `NULL` if any error occurs.
 */
char	*get_oldpwd_directory(t_ms *ms)
{
	char	*target;

	target = get_env_value("OLDPWD", ms->envp);
	if (!target)
	{
		ft_putstr_fd("cd: OLDPWD not set\n", STDERR_FILENO);
		ms->exit_status = 1;
		return (NULL);
	}
	if (*target == '\0')
	{
		handle_empty_oldpwd(ms);
		return (NULL);
	}
	if (access(target, F_OK) != 0)
	{
		print_cd_error(target, NO_FILE_OR_DIR);
		ms->exit_status = 1;
		return (NULL);
	}
	ft_putendl_fd(target, STDOUT_FILENO);
	return (ft_strdup(target));
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
static int	make_cd_args(char ***args, t_ms *ms, char *pwd_before)
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

/**
 * @brief Updates the `OLPWD` environment variable with the previous 
 *        working directory.
 * 
 * This function creates arguments to export the `OLPWD` environment variable, 
 * which stores the previous working directory before a `cd` command. If 
 * argument creation or memory allocation fails, an error is printed, and 
 * the exit status is set to `MALLOC_ERR`. The function then calls 
 * `handle_export` to update the `OLPWD` variable in the environment.
 * 
 * @param ms A pointer to the `t_ms` structure, which manages shell-related 
 *           data, including environment variables.
 * @param pwd_before A string representing the previous working directory.
 * 
 * @return None. The function updates the `OLPWD` environment variable and 
 *         modifies `ms->exit_status` on failure.
 */
static void	export_oldpwd(t_ms *ms, char *pwd_before)
{
	char	**args;

	args = NULL;
	if (!make_cd_args(&args, ms, pwd_before))
	{
		clean_arr(&args);
		print_malloc_set_status(ms);
		return ;
	}
	handle_export(args, ms);
	clean_arr(&args);
}

/**
 * @brief Adds the previous working directory (`OLDPWD`) to `envp` if it 
 * does not already exist.
 * 
 * This function checks if the `OLDPWD` variable exists in the `exported` 
 * environment. If `OLDPWD` is found in the `exported` array, the function 
 * proceeds to search for it in the `envp` array. If `OLDPWD` is not found 
 * in the `envp` array, it calls `make_cd_args` to add it, using the 
 * `pwd_before` argument as the value of `OLDPWD`.
 * 
 * The `OLDPWD` environment variable stores the previous working directory, 
 * which is updated during `cd` commands to allow users to return to the 
 * last directory they were in.
 * 
 * @param ms The main shell structure, containing both `envp` and 
 *           `exported` environment variables.
 * @param pwd_before The path to the directory before a `cd` command is 
 *                   executed (used as the `OLDPWD` value).
 */
void	add_oldpwd_to_envp(t_ms *ms, char *pwd_before)
{
	int		check;
	int		i;

	i = 0;
	check = 0;
	while (ms->exported[i])
	{
		if (!ft_strncmp("OLDPWD", ms->exported[i], 6))
			check = 1;
		i++;
	}
	if (check == 0)
		return ;
	i = 0;
	while (ms->envp[i])
	{
		if (!ft_strncmp("OLDPWD", ms->envp[i], 6))
			check = 0;
		i++;
	}
	if (check == 1)
		export_oldpwd(ms, pwd_before);
}
