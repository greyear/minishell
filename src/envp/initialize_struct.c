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

/**
 * @brief Initializes the environment variables in the `t_ms` structure.
 * 
 * This function copies the environment variables from `envp` into the 
 * `ms->envp` and `ms->exported` arrays in the `t_ms` structure.
 * 
 * If memory allocation for either `ms->envp` or `ms->exported` fails, 
 * the function prints an error message, cleans up the `t_ms` structure, 
 * and exits the program with a failure status.
 * 
 * @param ms Pointer to the `t_ms` structure where the environment variables will be stored.
 * @param envp The environment variables passed to the program.
 */

static void	initialize_envp(t_ms *ms, char **envp)
{
	if (!envp)
	{
		ms->envp = NULL;
		ms->exported = NULL;
		return;
	}
	ms->envp = copy_map(envp);
	if (!ms->envp)
	{
		perror("ms->envp: memory allocation failed");
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