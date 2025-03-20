#include "../../include/minishell.h"

/**
 * @brief Allocates memory and creates a duplicate of the given string.
 *
 * This function uses `ft_strdup` to allocate memory and copy the contents 
 * of `str` into a newly allocated string. If memory allocation fails, an 
 * error message is printed, and `NULL` is returned.
 *
 * @param str The string to duplicate.
 * @return A newly allocated copy of `str`, or `NULL` if allocation fails.
 */
char	*allocate_and_copy(const char *str)
{
	char	*new_str;

	new_str = ft_strdup(str);
	if (!new_str)
	{
		print_malloc_error();
		return (NULL);
	}
	return (new_str);
}

/**
 * @brief Initializes the `envp` array when no environment variables exist.
 *
 * This function manually allocates and sets up a minimal environment (`envp`) 
 * based on entries from the `exported` array. It ensures proper memory 
 * allocation and handles errors by setting the exit status if allocation fails.
 *
 * @param ms The shell structure containing execution state, including `envp` 
 *           and `exported` environment lists.
 */
static void	initialize_envp_without_envp(t_ms *ms)
{
	ms->envp = malloc(sizeof(char *) * 4);
	if (!ms->envp)
	{
		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
		return ;
	}
	ms->envp[0] = allocate_and_copy(ms->exported[1]);
	if (!ms->envp[0])
	{
		ms->exit_status = MALLOC_ERR;
		return ;
	}
	ms->envp[1] = allocate_and_copy(ms->exported[2]);
	if (!ms->envp[1])
	{
		ms->exit_status = MALLOC_ERR;
		return ;
	}
	ms->envp[2] = NULL;
}

static void	initialize_without_envp(t_ms *ms)
{
	char	cwd[1024];

	ms->exported = malloc(sizeof(char *) * 4);
	if (!ms->exported)
	{
		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
		return ;
	}
	ms->exported[0] = allocate_and_copy("OLDPWD");
	if (!ms->exported[0])
	{
		ms->exit_status = MALLOC_ERR;
		return ;
	}
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("pwd: getcwd failed");
		ms->exit_status = SYSTEM_ERR;
		return ;
	}
	ms->exported[1] = ft_strjoin("PWD=", cwd);
	if (!ms->exported[1])
	{
		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
		return ;
	}
	ms->exported[2] = allocate_and_copy("SHLVL=0");
	if (!ms->exported[2])
	{
		ms->exit_status = MALLOC_ERR;
		return ;
	}
	ms->exported[3] = NULL;
	initialize_envp_without_envp(ms);
}

/**
 * @brief Initializes the environment variables for the shell.
 *
 * This function sets up the `envp` and `exported` arrays by copying the 
 * provided environment (`envp`). If no environment is available, it 
 * initializes a minimal environment instead. It also sets a flag (`no_env`) 
 * to indicate whether an environment was inherited. If memory allocation 
 * fails, the shell structure is cleaned up, and the program exits with an 
 * error.
 *
 * @param ms The shell structure containing execution state.
 * @param envp The inherited environment variables from the system.
 */
void	initialize_envp(t_ms *ms, char **envp)
{
	if (!envp || !*envp)
	{
		initialize_without_envp(ms);
		ms->no_env = true;
		return ;
	}
	ms->no_env = false;
	ms->envp = copy_map(envp);
	if (!ms->envp)
	{
		print_malloc_error();
		clean_struct(ms);
		exit(1);
	}
	ms->exported = copy_map(envp);
	if (!ms->exported)
	{
		print_malloc_error();
		clean_struct(ms);
		exit(1);
	}
}
