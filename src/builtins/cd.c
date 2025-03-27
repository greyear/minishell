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
 * @brief Updates the `PWD` (current working directory) and `OLDPWD` 
 * (previous working directory) environment variables during a `cd` 
 * command.
 * 
 * This function performs the following actions:
 * - Retrieves the current working directory using `getcwd()` and stores 
 *   it in `PWD`.
 * - Checks if the `PWD` environment variable is set. If it is, it updates 
 *   the `OLDPWD` environment variable to store the previous value of `PWD`. 
 *   If `PWD` isn't set or is empty, it uses the `pwd_before` argument as 
 *   the value for `OLDPWD`.
 * - If the `OLDPWD` environment variable doesn't exist and the shell is 
 *   not using `envp`, it calls `add_oldpwd_to_envp` to add `OLDPWD` to 
 *   the environment.
 * - Finally, it updates the `PWD` environment variable to the newly 
 *   retrieved `cwd`.
 * 
 * The `PWD` variable reflects the current working directory, and `OLDPWD` 
 * stores the previous working directory to allow users to return to it 
 * using the `cd -` command.
 * 
 * @param ms The main shell structure, containing the environment variables 
 *           `envp` and flags like `no_env`.
 * @param pwd_before The value of `PWD` before the `cd` operation, used for 
 *                   updating `OLDPWD` when necessary.
 */
void	update_cd_env(t_ms *ms, char *pwd_before)
{
	char	cwd[1024];
	char	*current_pwd;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("cd: getcwd failed");
		ms->exit_status = 1;
		return ;
	}
	current_pwd = get_env_value("PWD", ms->envp);
	if (!current_pwd)
		current_pwd = "";
	if (current_pwd && *current_pwd != '\0')
		update_env_var(ms, "OLDPWD=", current_pwd);
	else
		update_env_var(ms, "OLDPWD=", pwd_before);
	if (ms->exit_status != MALLOC_ERR
		&& !get_env_value("OLDPWD", ms->envp)
		&& ms->no_env == true)
		add_oldpwd_to_envp(ms, pwd_before);
	if (ms->exit_status != MALLOC_ERR)
		update_env_var(ms, "PWD=", cwd);
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
	update_cd_env(ms, ms->pwd);
	if (ms->pwd)
		free(ms->pwd);
	ms->pwd = ft_strdup(target_dir);
	free(target_dir);
}
