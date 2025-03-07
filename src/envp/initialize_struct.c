#include "../../include/minishell.h"

/**
 * @brief Initializes the ms structure.
 * 
 * Allocates memory for the `t_ms` structure and initializes its fields, 
 * including environment variables, command history, and history file status.
 * 
 * @param envp A pointer to the environment variables array.
 * 
 * @return A pointer to the initialized `t_ms` structure, or NULL if allocation fails.
 */
/*t_ms	*initialize_struct(char **envp)
{
	t_ms *ms;

	ms = malloc(sizeof(t_ms));
	if (!ms)
		return (NULL);
	ms->exit_status = 0;
	ms->envp = copy_map(envp);
	ms->exported = copy_map(envp);
	default_history(ms->history);
	ms->history_num = 0;
	if (open_read_history_file(ms) == 0)
		ms->history_file = true;
	else
	{
		print_system_error(HIST_ERR);
		ms->history_file = false;
	}
	ms->heredoc_count = 0;
	ms->heredoc_files = malloc(sizeof(char *) * 100); // Support 100 heredocs max
	ms->heredoc_files[0] = NULL;
	ft_memset(ms->heredoc_files, 0, sizeof(char *) * 100); // Set all entries to NULL
	if (!ms->heredoc_files)
	{
		perror("heredoc: memory allocation failed");
		exit(1);

	}
	return (ms);
}*/

static t_ms	*allocate_struct(void)
{
	t_ms *ms;

	ms = malloc(sizeof(t_ms));
	if (!ms)
	{
		perror("malloc failed for t_ms");
		return (NULL);
	}
	ms->exit_status = 0;
	return (ms);
}

static void	initialize_envp(t_ms *ms, char **envp)
{
	ms->envp = copy_map(envp);
	ms->exported = copy_map(envp);
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

static void	initialize_heredoc(t_ms *ms)
{
	ms->heredoc_count = 0;
	ms->heredoc_files = malloc(sizeof(char *) * 100); // Support 100 heredocs max
	if (!ms->heredoc_files)
	{
		perror("heredoc: memory allocation failed");
		exit(1);
	}
	ft_memset(ms->heredoc_files, 0, sizeof(char *) * 100); // Set all entries to NULL
	ms->heredoc_files[0] = NULL;
}

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
	initialize_heredoc(ms);
	ms->saved_stdin = dup(STDIN_FILENO);
	ms->saved_stdout = dup(STDOUT_FILENO);
	return (ms);
}