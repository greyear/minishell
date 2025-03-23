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
 * @brief Initializes all fields of the minishell structure to NULL or default values.
 *
 * This function sets each field of the `t_ms` structure to a NULL pointer or its 
 * appropriate default value (such as 0 for integer fields). It ensures that all 
 * fields are properly initialized before they are used in further processing, 
 * preventing potential issues with uninitialized memory.
 *
 * @param ms The minishell structure to be initialized.
 */
static void	initialize_to_null(t_ms *ms)
{
	ms->tokens = NULL;
	ms->blocks = NULL;
	ms->cmds = NULL;
	ms->envp = NULL;
	ms->exported = NULL;
	ms->heredoc_count = 0;
	ms->heredoc_files = NULL;
	ms->pwd = NULL;
}

/**
 * @brief Initializes the minishell structure.
 *
 * This function allocates and initializes a `t_ms` structure, setting its 
 * fields to their default values and ensuring that necessary resources (such 
 * as environment variables, history, and shell level) are properly set up. 
 * It attempts to retrieve the current working directory and assigns it to the 
 * `pwd` field. If memory allocation for `pwd` fails, an error is printed, 
 * and the program exits. The function also sets up the environment variables, 
 * shell level, and history through additional helper functions.
 *
 * @param envp The environment variables passed to the program.
 * 
 * @return A pointer to the initialized `t_ms` structure, or `NULL` if memory 
 * allocation fails.
 */
t_ms	*initialize_struct(char **envp)
{
	t_ms	*ms;

	ms = allocate_struct();
	if (!ms)
		return (NULL);
	initialize_to_null(ms);
	getcwd(ms->pwd, sizeof(ms->pwd));
	if (!ms->pwd)
		ms->pwd = ft_strdup("");
	if (!ms->pwd)
	{
		print_malloc_error();
		free(ms);
		exit(1);
	}
	initialize_envp_and_exp(ms, envp);
	initialize_history(ms);
	update_shlvl(ms);
	g_sgnl = 0;
	return (ms);
}
