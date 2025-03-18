#include "../../include/minishell.h"

/**
 * @brief Allocates memory for a new `t_ms` structure and initializes it.
 * 
 * This function allocates memory for a `t_ms` structure and initializes the 
 * `exit_status` field to 0. If memory allocation fails, it prints an error 
 * message and exits the program.
 * 
 * @return A pointer to the newly allocated `t_ms` structure.
 */

static t_ms	*allocate_struct(void)
{
	t_ms	*ms;

	ms = malloc(sizeof(t_ms));
	if (!ms)
	{
		perror("t_ms: memory allocation failed");
		exit(1);
	}
	ms->exit_status = 0;
	return (ms);
}

static void	initialize_envp_without_envp(t_ms *ms)
{
	ms->envp = malloc(sizeof(char *) * 4);
	if (!ms->envp)
	{
		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
		return;
	}
	ms->envp[0] = ft_strdup(ms->exported[1]);
	if (!ms->envp[0])
	{
		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
		return;
	}
	ms->envp[1] = ft_strdup(ms->exported[2]);
	if (!ms->envp[1])
	{
		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
		return;
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
		return;
	}
	ms->exported[0] = ft_strdup("OLDPWD");
	if (!ms->exported[0])
	{
		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
		return;
	}
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("pwd: getcwd failed");
		ms->exit_status = SYSTEM_ERR;
		return;
	}
	ms->exported[1] = ft_strjoin("PWD=", cwd);
	if (!ms->exported[1])
	{
		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
		return;
	}
	ms->exported[2] = ft_strdup("SHLVL=0");
	if (!ms->exported[2])
	{
		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
		return;
	}
	ms->exported[3] = NULL;
	initialize_envp_without_envp(ms);
}

static void	initialize_envp(t_ms *ms, char **envp)
{
	if (!envp || !*envp)
	{
		initialize_without_envp(ms);
		ms->no_env = true;
		return;
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

static void	initialize_history(t_ms *ms)
{
	default_history(ms->history);
	ms->history_num = 0;
	if (open_read_history_file(ms) == 0)
		ms->history_file = true;
	else
	{
		print_system_error(HIST_ERR);
		ms->history_file = false;
	}
}

/**
 * @brief Initializes and allocates memory for the main shell structure.
 * 
 * This function:
 * - Allocates memory for a `t_ms` structure.
 * - Initializes environment variables (`envp`).
 * - Sets up tokens and command blocks.
 * - Initializes command history.
 * - Prepares file descriptors and heredoc-related data.
 * - Updates SHLVL in environmental variables if needed.
 * - Updates SHLVL in environmental variables if needed.
 * 
 * @param envp The environment variables inherited from the parent process.
 * 
 * @return A pointer to the initialized `t_ms` structure, or `NULL` if allocation fails.
 * 
 */

t_ms	*initialize_struct(char **envp)
{
	t_ms *ms;

	ms = allocate_struct();
	if (!ms)
		return (NULL);
	initialize_envp(ms, envp);
	ms->tokens = NULL;
	ms->blocks = NULL;
	initialize_history(ms);
	ms->heredoc_count = 0;
	ms->heredoc_files = NULL;
	check_shlvl(ms);
	g_sgnl = 0;
	return (ms);
}