#include "../../include/minishell.h"

static t_ms	*allocate_struct(void)
{
	t_ms *ms;

	ms = malloc(sizeof(t_ms));
	if (!ms)
	{
		perror("t_ms: memory allocation failed");
		exit(1);
	}
	ms->exit_status = 0;
	return (ms);
}

static void	initialize_envp(t_ms *ms, char **envp)
{
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
		perror("ms->exported: memory allocation failed");
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

static void	initialize_heredoc(t_ms *ms)
{
	ms->heredoc_count = 0;
	/*ms->heredoc_files = malloc(sizeof(char *) * 100); // Support 100 heredocs max
	if (!ms->heredoc_files)
	{
		perror("heredoc: memory allocation failed");
		clean_struct(ms);
		exit(1);
	}
	ft_memset(ms->heredoc_files, 0, sizeof(char *) * 100); // Set all entries to NULL
	ms->heredoc_files[0] = NULL;*/
	ms->heredoc_files = NULL;
}

static void	initialize_fds(t_ms *ms)
{
	ms->saved_stdin = dup(STDIN_FILENO);
	if (ms->saved_stdin == -1)
	{
		perror("dup stdin failed");
		clean_struct(ms);
		exit(1);
	}
	ms->saved_stdout = dup(STDOUT_FILENO);
	if (ms->saved_stdout == -1)
	{
		perror("dup stdout failed");
		clean_struct(ms);
		exit(1);
	}
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
	initialize_fds(ms);
	return (ms);
}