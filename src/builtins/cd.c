#include "../../include/minishell.h"

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
 * @brief Updates the shell's environment variables after a directory change.
 * 
 * This function updates the `PWD` and `OLDPWD` environment variables after 
 * a successful `cd` command. It retrieves the current working directory 
 * using `getcwd()` and sets `PWD` to the new directory. If `OLDPWD` was 
 * previously set, it updates it with the previous `PWD` value; otherwise, 
 * it uses the provided `pwd_before` value.
 * 
 * @param ms A pointer to the `t_ms` structure, which contains the shell's 
 *           environment variables.
 * @param pwd_before The directory path before changing to the new one.
 * 
 * @return None. Modifies the environment variables in `ms->envp` and 
 *         updates `ms->exit_status` in case of an error.
 */

static void	update_cd_env(t_ms *ms, char *pwd_before)
{
	char	cwd[1024];
	char	*current_pwd;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("cd: getcwd failed");
		ms->exit_status = 1;
		return;
	}
	current_pwd = get_env_value("PWD", ms->envp);
	if (!current_pwd)
		current_pwd = "";
	if (current_pwd && *current_pwd != '\0')
		update_env_var(ms, "OLDPWD=", current_pwd);
	else
		update_env_var(ms, "OLDPWD=", pwd_before);
	if (ms->exit_status == 1) //in case of malloc error
		return;
	update_env_var(ms, "PWD=", cwd);
}

/**
 * @brief Updates or adds an environment variable in the shell's environment.
 * 
 * This function searches for an existing environment variable matching the given 
 * key. If found, it updates its value. If the variable does not exist, no new 
 * entry is added. Memory for the new value is dynamically allocated and replaces 
 * the old entry. 
 * 
 * @param ms A pointer to the `t_ms` structure, which contains the shell's 
 *           environment variables.
 * @param key The name of the environment variable to update.
 * @param new_value The new value to assign to the environment variable.
 * 
 * @return None. Modifies the environment variables stored in `ms->envp`.
 *         If memory allocation fails, the function does nothing.
 */

void	update_env_var(t_ms *ms, char *key, char *new_value)
{
	int	i;
	char	*new_env_entry;

	if (!ms->envp)
		return;
	i = 0;
	new_env_entry = malloc(ft_strlen(key) + ft_strlen(new_value) + 1);
	if (!new_env_entry)
	{
		print_malloc_error();
		ms->exit_status = 1;
		return;
	}
	ft_strcpy(new_env_entry, key);
	ft_strcat(new_env_entry, new_value);
	while (ms->envp[i])
	{
		if (ft_strncmp(ms->envp[i], key, ft_strlen(key)) == 0)
		{
			free(ms->envp[i]);
			ms->envp[i] = new_env_entry;
			return;
		}
		i++;
	}
}

/**
 * @brief Checks for errors in the `cd` command arguments.
 * 
 * This function verifies if the provided arguments for `cd` are valid. It 
 * ensures that the command is correctly formatted and checks for the presence 
 * of too many arguments. If an error is detected, an appropriate error message 
 * is printed, and the shell's exit status is updated.
 * 
 * @param args An array of command arguments, where args[0] should be "cd".
 * @param ms A pointer to the `t_ms` structure, which holds shell-related data, 
 *           including the exit status.
 * 
 * @return `1` if an error occurs (e.g., invalid arguments, too many arguments), 
 *         otherwise `0` if the arguments are valid.
 */

static int	cd_error(char **args, t_ms *ms)
{
	if (!args || !*args)
		return (1);	
	if (ft_strcmp(args[0], "cd") != 0)
		return (1);
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
 * @brief Handles the execution of the `cd` command.
 * 
 * This function changes the current working directory based on the provided 
 * arguments. It first checks for errors and retrieves the target directory. 
 * If an error occurs or the target directory is invalid, it returns without 
 * making changes. Otherwise, it attempts to change the directory using `chdir()`. 
 * If successful, it updates the shell's environment variables accordingly.
 * 
 * @param args An array of arguments passed to the `cd` command, where args[1] 
 *             typically contains the target directory.
 * @param ms A pointer to the `t_ms` structure, which holds environment variables 
 *           and shell-related data.
 * 
 * @return None. Updates the environment variables and exit status in `ms`.
 */

void	handle_cd(char **args, t_ms *ms)
{
	char	*target_dir;
	char	cwd[1024];

	ms->exit_status = 0;
	if (cd_error(args, ms))
		return;
	target_dir = get_cd_target(ms, args);
	if (ms->exit_status == 1 || !target_dir)
		return;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		free(target_dir);
		perror("getcwd failed\n");
		ms->exit_status = 1;
		return;
	}
	if (chdir(target_dir) == -1)
	{
		free(target_dir);
		perror("chdir failed\n");
		ms->exit_status = 1;
		return;
	}
	free(target_dir);
	update_cd_env(ms, cwd);
}