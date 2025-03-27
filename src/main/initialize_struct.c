/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_struct.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 14:07:41 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 14:07:43 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		print_system_error(HIST_RD_ERR);
		ms->history_file = false;
	}
}

/**
 * @brief Initializes all fields of the minishell structure to NULL or 
 *        default values.
 *
 * This function sets each field of the `t_ms` structure to a NULL pointer or 
 * its appropriate default value (such as 0 for integer fields). It ensures 
 * that all fields are properly initialized before they are used in further
 * processing, preventing potential issues with uninitialized memory.
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
 * @brief Initializes the main Minishell structure.
 * 
 * This function allocates and initializes the `t_ms` structure, setting up 
 * environment variables, history, and shell level (`SHLVL`). It also retrieves 
 * the current working directory (`PWD`). If memory allocation fails or `getcwd` 
 * encounters an error, the function prints an error message, frees allocated 
 * resources, and exits the program.
 * 
 * @param envp The environment variables passed to the shell.
 * 
 * @return A pointer to the initialized `t_ms` structure, or `NULL` if 
 *         allocation fails.
 */
t_ms	*initialize_struct(char **envp)
{
	t_ms	*ms;
	char	cwd[1024];

	ms = allocate_struct();
	if (!ms)
		return (NULL);
	initialize_to_null(ms);
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd failed");
		free(ms);
		exit(1);
	}
	ms->pwd = ft_strdup(cwd);
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
