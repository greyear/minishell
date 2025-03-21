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
 * @brief Initializes the shell's history functionality.
 *
 * This function sets up the shell's history by calling `default_history` to 
 * set default values for the history list and initializing the history number. 
 * It then attempts to open the history file for reading. If the file is 
 * successfully opened, the history file flag is set. Otherwise, an error 
 * message is printed and the history file flag is set to false.
 *
 * @param ms The shell structure containing execution state, including the 
 *           history.
 */
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
 * 
 * @param envp The environment variables inherited from the parent process.
 * 
 * @return A pointer to the initialized `t_ms` structure, 
 *         or `NULL` if allocation fails.
 * 
 */
t_ms	*initialize_struct(char **envp)
{
	t_ms	*ms;

	ms = allocate_struct();
	if (!ms)
		return (NULL);
	initialize_envp(ms, envp);
	ms->tokens = NULL;
	ms->blocks = NULL;
	initialize_history(ms);
	ms->heredoc_count = 0;
	ms->heredoc_files = NULL;
	ms->pwd = NULL;
	update_shlvl(ms);
	g_sgnl = 0;
	return (ms);
}
