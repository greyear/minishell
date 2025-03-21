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
 * @brief Changes the current working directory to the specified target.
 * 
 * This function attempts to change the working directory using the `chdir`
 * system call. If the operation fails (e.g., if the target directory does 
 * not exist or the process lacks permission), it prints an error message, 
 * frees the target directory string, updates the shell's exit status to 
 * indicate a system error, and returns a non-zero value.
 * 
 * @param target_dir The target directory to change to. This string is 
 *                   dynamically allocated and will be freed if the 
 *                   operation fails.
 * @param ms A pointer to the `t_ms` structure, which contains environment 
 *           variables and shell-related data, including the exit status.
 * 
 * @return 0 if the directory change is successful, or a non-zero value if 
 *         the operation fails.
 */
static int	change_directory(char *target_dir, t_ms *ms)
{
	if (chdir(target_dir) == -1)
	{
		free(target_dir);
		perror("chdir failed");
		ms->exit_status = SYSTEM_ERR;
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
 * @return `1` if an error occurs (e.g., invalid arguments, too many arguments), 
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
	if (!ft_strcmp(args[1], "~"))
		return (1);
	return (0);
}

/**
 * @brief Handles the `cd` (change directory) command in the shell.
 * 
 * This function processes the `cd` command by performing various checks 
 * such as verifying the target directory, ensuring it is valid, and 
 * handling errors (e.g., failure to retrieve the current directory, 
 * invalid directory, etc.). It updates the shell's environment variables 
 * (`PWD` and `OLDPWD`) and changes the working directory if everything 
 * is valid. On failure, it updates the shell's exit status and returns 
 * early.
 * 
 * @param args An array of strings representing the command and its 
 *             arguments. The first argument is expected to be `cd`, and 
 *             the second is the target directory.
 * @param ms A pointer to the `t_ms` structure, which contains environment 
 *           variables and shell-related data, including the exit status.
 * 
 * @return None. The function modifies `ms->exit_status` and updates the 
 *         shell's environment.
 */
void	handle_cd(char **args, t_ms *ms)
{
	char	*target_dir;
	char	*pwd;

	ms->exit_status = 0;
	if (cd_error(args, ms))
		return ;
	target_dir = get_cd_target(ms, args);
	if (!target_dir)
		return ;
	if (handle_cd_directory_checks(target_dir, ms))
		return ;
	pwd = get_env_value("PWD", ms->envp);
	if (change_directory(target_dir, ms))
		return ;
	free(target_dir);
	update_cd_env(ms, pwd);
}
