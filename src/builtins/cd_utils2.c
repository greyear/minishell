#include "../../include/minishell.h"

/**
 * @brief Handles the case where OLDPWD is empty.
 * 
 * This function retrieves the current working directory
 * from the PWD environment variable. If PWD is not set or empty,
 * it attempts to get the directory using 
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
			perror("getcwd failed");
			ms->exit_status = 1;
			return (NULL);
		}
		update_env_var(ms, "PWD=", cwd);
		return (NULL);
	}
	update_env_var(ms, "OLDPWD=", current_pwd);
	return (ft_strdup(current_pwd));
}

/**
 * @brief Retrieves the previous working directory stored in the `OLDPWD` 
 * environment variable.
 * 
 * This function attempts to fetch the value of the `OLDPWD` environment 
 * variable and perform several checks:
 * - If `OLDPWD` is not set, it prints an error message and returns `NULL`.
 * - If `OLDPWD` is empty, it calls `handle_empty_oldpwd` to handle this case.
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
		return (handle_empty_oldpwd(ms));
	if (access(target, F_OK) != 0)
	{
		print_cd_error(target, NO_FILE_OR_DIR);
		ms->exit_status = 1;
		return (NULL);
	}
	ft_putendl_fd(target, STDOUT_FILENO);
	return (ft_strdup(target));
}

static void	make_cd_args(t_ms *ms, char *pwd_before, int i)
{
	char	**args;

	args = malloc(sizeof(char *) * 3);
	if (!args)
	{
		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
		return ;
	}
	while (i < 2)
	{
		if (i == 0)
			args[i] = ft_strdup("export");
		if (i == 1)
			args[i] = ft_strjoin("OLDPWD=", pwd_before);
		if (!args[i])
		{
			clean_arr(&args);
			print_malloc_error();
			ms->exit_status = MALLOC_ERR;
			return ;
		}
		i++;
	}
	args[2] = NULL;
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
static void	add_oldpwd_to_envp(t_ms *ms, char *pwd_before)
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
		make_cd_args(ms, pwd_before, 0);
}

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
