#include "../../include/minishell.h"

/**
 * @brief Handles the case where the OLDPWD environment variable is missing.
 * 
 * This function prints an error message indicating that OLDPWD is not set, 
 * updates the shell's exit status to 1, and returns NULL.
 * 
 * @param ms A pointer to the `t_ms` structure, which contains the exit status 
 *           and other necessary shell-related data.
 * 
 * @return Always returns `NULL`, as OLDPWD is missing.
 */

static char	*handle_missing_oldpwd(t_ms *ms)
{
	ft_putstr_fd("cd: OLDPWD not set\n", STDERR_FILENO);
	ms->exit_status = 1;
	return (NULL);
}

/**
 * @brief Handles the case where OLDPWD is empty.
 * 
 * This function retrieves the current working directory from the PWD environment 
 * variable. If PWD is not set or empty, it attempts to get the directory using 
 * getcwd() and updates PWD accordingly. Otherwise, it updates OLDPWD with the 
 * current PWD value.
 * 
 * @param ms A pointer to the `t_ms` structure, which contains environment 
 *           variables and shell-related data.
 * 
 * @return A newly allocated string containing the current working directory 
 *         (previously stored in PWD) or NULL if PWD was empty and updated 
 *         from getcwd().
 */

static char	*handle_empty_oldpwd(t_ms *ms)
{
	char	*current_pwd;
	char	cwd[1024];

	ft_putstr_fd("\n", STDOUT_FILENO);
	current_pwd = get_env_value("PWD", ms->envp);
	if (!current_pwd || *current_pwd == '\0')
	{
		if (getcwd(cwd, sizeof(cwd)) == NULL)
		{
			perror("getcwd failed\n");
			ms->exit_status = SYSTEM_ERR;
			return (NULL);
		}
		update_env_var(ms, "PWD=", cwd);
		return (NULL);
	}
	update_env_var(ms, "OLDPWD=", current_pwd);
	return (ft_strdup(current_pwd));
}

/**
 * @brief Validates and returns the target directory for the `cd` command.
 * 
 * This function checks if the specified `target` directory exists using `access(F_OK)`. 
 * If the directory does not exist, it prints an error message and updates the shell's 
 * exit status. Otherwise, it prints the directory path and returns a dynamically allocated 
 * copy of it.
 * 
 * @param ms A pointer to the shell structure containing execution state.
 * @param target The directory path to validate and return.
 * 
 * @return A newly allocated string containing `target` if it exists, or NULL on failure.
 */

 char	*return_target(t_ms *ms, char *target)
 {
	 if (access(target, F_OK) != 0)
	 {
		 print_cd_error(target);
		 ms->exit_status = 1;
		 return (NULL);
	 }
	 ft_putendl_fd(target, STDOUT_FILENO);
	 return (ft_strdup(target));
 }

/**
 * @brief Retrieves the OLDPWD directory.
 * 
 * This function attempts to get the OLDPWD value from the environment. 
 * If OLDPWD is not set, it handles the missing value by displaying an error message. 
 * If OLDPWD is an empty string, it processes the case accordingly by using the 
 * current working directory. Otherwise, it returns the OLDPWD value.
 * 
 * @param ms A pointer to the `t_ms` structure, which contains environment 
 *           variables and shell-related data.
 * 
 * @return A newly allocated string containing the OLDPWD value, a fallback 
 *         directory if OLDPWD is empty, or NULL if OLDPWD is missing.
 */

char	*get_oldpwd_directory(t_ms *ms)
{
	char	*target;

	target = get_env_value("OLDPWD", ms->envp);
	if (!target)
		return (handle_missing_oldpwd(ms));
	if (*target == '\0')
		return (handle_empty_oldpwd(ms));
	return (return_target(ms, target));
}