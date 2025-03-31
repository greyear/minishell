/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 12:55:20 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 12:55:22 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Updates the `PWD` and `OLDPWD` environment variables.
 *
 * This function updates the shell's `PWD` and `OLDPWD` environment variables
 * based on the current and previous working directories after a `cd` operation.
 * It handles cases where `OLDPWD` might be missing and ensures that environment
 * variables are updated accordingly.
 * 
 * - If `OLDPWD` is not set, it adds the previous working directory to the
 *   environment.
 * - If the `PWD` value is present, it updates `OLDPWD` with the current
 *   directory.
 * - If `PWD` is not found, and the variable `unset_pwd_exp_old` is true, it
 *   removes `OLDPWD` from the environment and re-adds it.
 * - It updates `PWD` to the new working directory.
 * 
 * @param ms A pointer to the main shell structure containing environment
 *           variables.
 * @param pwd_before The previous working directory (before `cd`).
 * @param pwd_now The current working directory (after `cd`).
 */
void	update_cd_env(t_ms *ms, char *pwd_before, char *pwd_now)
{
	char	*current_pwd;

	if (ms->exit_status != MALLOC_ERR
		&& !get_env_value("OLDPWD", ms->envp))
		add_oldpwd_to_envp(ms, pwd_before);
	current_pwd = get_env_value("PWD", ms->envp);
	if (current_pwd)
		update_env_var(ms, "OLDPWD=", current_pwd);
	else if (ms->unset_pwd_exp_old == true)
	{
		if (!rm_from_env_ex(&ms->exported, "OLDPWD", 1)
			|| !rm_from_env_ex(&ms->envp, "OLDPWD", 0))
			print_malloc_set_status(ms);
		if (ms->exit_status != MALLOC_ERR)
			add_to_exported("OLDPWD", ms);
		ms->unset_pwd_exp_old = false;
	}
	else
		update_env_var(ms, "OLDPWD=", pwd_before);
	if (ms->exit_status != MALLOC_ERR)
		update_env_var(ms, "PWD=", pwd_now);
}

/**
 * @brief Determines the target directory for the `cd` command.
 * 
 * This function resolves the directory path based on the given arguments. 
 * If no argument is provided, it returns the user's home directory. If the 
 * argument is "-", it returns the previous working directory (`OLDPWD`). 
 * If the argument is "..", it returns the parent directory. For relative paths, 
 * it constructs the full path using the current working directory. Otherwise, 
 * it returns an absolute path as is.
 * 
 * @param ms A pointer to the `t_ms` structure, which contains the shell's 
 *           environment variables and exit status.
 * @param args An array of command arguments, where args[1] specifies the 
 *             target directory.
 * 
 * @return A dynamically allocated string containing the resolved directory 
 *         path, or NULL if an error occurs. The caller is responsible for 
 *         freeing the returned string.
 */
static char	*get_cd_target(t_ms *ms, char **args)
{
	char	cwd[1024];

	if (!args[1])
		return (get_home_directory(ms, 0));
	if (ft_strcmp(args[1], "-") == 0)
		return (get_oldpwd_directory(ms));
	if (ft_strcmp(args[1], "..") == 0)
		return (get_parent_directory(ms));
	if (args[1][0] != '/')
	{
		if (getcwd(cwd, sizeof(cwd)) == NULL)
		{
			perror("cd: getcwd failed");
			ms->exit_status = 1;
			return (NULL);
		}
		return (build_relative_path(args[1], cwd, ms));
	}
	return (ft_strdup(args[1]));
}

/**
 * @brief Performs checks on the target directory before attempting 
 * to change to it.
 * 
 * This function checks whether the target directory exists and whether
 * the current process has the necessary permissions to access it. 
 * If the directory does not exist or if access is denied, 
 * an appropriate error message is printed, target_dir freed, 
 * the shell's exit status is updated, and the function returns a non-zero value.
 * If the directory passes both checks, the function returns 0,
 * indicating that the directory is valid for a change.
 * 
 * @param target_dir The directory path to check.
 * @param ms A pointer to the `t_ms` structure, which contains environment
 *           variables and shell-related data, including the exit status.
 * 
 * @return 0 if the directory is valid for changing to, or a non-zero value
 *  if the checks fail.
 */
static int	handle_cd_directory_checks(char *target_dir, t_ms *ms)
{
	if (access(target_dir, F_OK) == -1)
	{
		print_cd_error(target_dir, NO_FILE_OR_DIR);
		free(target_dir);
		ms->exit_status = 1;
		return (1);
	}
	if (access(target_dir, X_OK) == -1)
	{
		print_cd_error(target_dir, PERM_DEN);
		free(target_dir);
		ms->exit_status = 1;
		return (1);
	}
	return (0);
}

/**
 * @brief Checks for errors in the `cd` command arguments.
 * 
 * This function verifies if the provided arguments for `cd` are valid. It 
 * ensures that the command is correctly formatted and checks for the presence 
 * of too many arguments. If an error is detected, an appropriate error message 
 * is printed, and the shell's exit status is updated.
 * 
 * @param args An array of command arguments.
 * @param ms A pointer to the `t_ms` structure, which holds shell-related data, 
 *           including the exit status.
 * 
 * @return `1` if an error occurs (too many arguments), 
 *         otherwise `0` if the arguments are valid.
 */
static int	cd_error(char **args, t_ms *ms)
{
	if (!args[1])
		return (0);
	if (args[2])
	{
		ft_putstr_fd(OWN_ERR_MSG, STDERR_FILENO);
		ft_putstr_fd("cd: too many arguments\n", STDERR_FILENO);
		ms->exit_status = 1;
		return (1);
	}
	return (0);
}

/**
 * @brief Handles the `cd` command to change the current directory.
 * 
 * This function checks for errors in the `cd` command arguments, determines 
 * the target directory, performs directory checks, and attempts to change 
 * the directory using `chdir`. If successful, it updates the shell's `PWD` 
 * environment variable. On failure, it prints an error message and sets 
 * the appropriate exit status.
 * 
 * @param args An array of strings representing the arguments passed to `cd`. 
 *             The first argument is the target directory.
 * @param ms A pointer to the `t_ms` structure, which manages shell-related 
 *           data, including exit status and current working directory (`pwd`).
 * 
 * @return None. The function modifies `ms->exit_status`, `ms->pwd`, and 
 *         updates the environment variables.
 */
void	handle_cd(char **args, t_ms *ms)
{
	char	*target_dir;

	ms->exit_status = 0;
	if (cd_error(args, ms))
		return ;
	target_dir = get_cd_target(ms, args);
	if (!target_dir)
		return ;
	if (handle_cd_directory_checks(target_dir, ms))
		return ;
	if (chdir(target_dir) == -1)
	{
		free(target_dir);
		perror("chdir failed");
		ms->exit_status = 1;
		return ;
	}
	update_cd_env(ms, ms->pwd, target_dir);
	if (ms->pwd)
		free(ms->pwd);
	ms->pwd = ft_strdup(target_dir);
	free(target_dir);
}
