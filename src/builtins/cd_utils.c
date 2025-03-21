#include "../../include/minishell.h"

/**
 * @brief Retrieves the home directory path.
 * 
 * This function attempts to get the home directory from the environment 
 * variable `HOME`. If `HOME` is not set, it optionally falls back to 
 * `getenv("HOME")` based on the `flag`. If the directory is empty or does 
 * not exist, an error is printed, and the shell's exit status is updated.
 * 
 * @param ms A pointer to the shell structure containing execution state.
 * @param flag If set to 1 and `envp` exists, attempts to use 
 *            `getenv("HOME")` as a fallback.
 * 
 * @return A newly allocated string containing the home directory path, 
 *         or `NULL` on failure.
 */
char	*get_home_directory(t_ms *ms, int flag)
{
	char	*temp;

	temp = get_env_value("HOME", ms->envp);
	if (!temp)
	{
		if (flag == 1 && ms->no_env == false)
			return (ft_strdup(getenv("HOME")));
		ft_putstr_fd("bash: cd: HOME not set\n", STDERR_FILENO);
		ms->exit_status = 1;
		return (NULL);
	}
	if (*temp == '\0')
	{
		ms->exit_status = 1;
		return (NULL);
	}
	if (access(temp, F_OK) != 0)
	{
		print_cd_error(temp, NO_FILE_OR_DIR);
		ms->exit_status = 1;
		return (NULL);
	}
	return (ft_strdup(temp));
}

/**
 * @brief Retrieves the parent directory of the current working directory.
 * 
 * This function uses `getcwd` to obtain the current directory and then 
 * determines the parent directory by locating the last `/` separator.
 * If the parent directory exists and is not the root, it is extracted; 
 * otherwise, `/` is returned as the parent.
 * 
 * @param ms A pointer to the shell structure containing execution state.
 * 
 * @return A newly allocated string containing the parent directory path, 
 *         or NULL if `getcwd` fails.
 */
char	*get_parent_directory(t_ms *ms)
{
	char	cwd[1024];
	char	*parent_dir;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd failed");
		ms->exit_status = 1;
		return (NULL);
	}
	parent_dir = ft_strrchr(cwd, '/');
	if (parent_dir && parent_dir != cwd)
	{
		*parent_dir = '\0';
		return (ft_strdup(cwd));
	}
	return (ft_strdup("/"));
}

/**
 * @brief Constructs a relative path by appending the target to the 
 *        current directory.
 * 
 * This function concatenates the current working directory (`cwd`) with 
 * a given `target` directory or file, ensuring a proper path format with 
 * a `/` separator.
 * 
 * @param target The relative target path to append to `cwd`.
 * @param cwd The current working directory.
 * 
 * @return A newly allocated string containing the full relative path, or 
 *         `NULL` if memory allocation fails.
 */
char	*build_relative_path(char *target, char *cwd, t_ms *ms)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(cwd, "/");
	if (!temp)
	{
		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
		return (NULL);
	}
	full_path = ft_strjoin(temp, target);
	if (!full_path)
	{
		free(temp);
		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
		return (NULL);
	}
	free(temp);
	return (full_path);
}

/**
 * @brief Updates or adds an environment variable in the shell's environment.
 * 
 * This function searches for an existing environment variable matching
 * the given key. If found, it updates its value.
 * If the variable does not exist, no new entry is added.
 * Memory for the new value is dynamically allocated and replaces 
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
	int		i;
	char	*new_env_entry;

	i = 0;
	new_env_entry = malloc(ft_strlen(key) + ft_strlen(new_value) + 1);
	if (!new_env_entry)
	{
		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
		return ;
	}
	ft_strcpy(new_env_entry, key);
	ft_strcat(new_env_entry, new_value);
	while (ms->envp[i])
	{
		if (ft_strncmp(ms->envp[i], key, ft_strlen(key)) == 0)
		{
			free(ms->envp[i]);
			ms->envp[i] = new_env_entry;
			return ;
		}
		i++;
	}
}
