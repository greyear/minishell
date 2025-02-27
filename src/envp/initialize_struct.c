#include "../../include/minishell.h"

t_ms	*initialize_struct(char **envp)
{
	t_ms *ms;

	ms = malloc(sizeof(t_ms));
	if (!ms)
		return (NULL);
	ms->exit_status = 0;
	ms->envp = copy_map(envp);
	ms->exported = copy_map(envp);
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
}

void	clean_struct(t_ms *ms)
{
	if (!ms)
		return ;
	if (ms->envp)
	{
		clean_arr(&(ms->envp));
		//ft_printf(2, "Freeing envp...\n");
	}
	if (ms->exported)
	{
		clean_arr(&(ms->exported));
		//ft_printf(2, "Freeing exported...\n");
	}
	free(ms);
}

void	clean_struct_fields(t_ms *ms)
{
	if (!ms)
		return ;
	if (ms->envp)
	{
		clean_arr(&(ms->envp));
		//ft_printf(2, "Freeing envp...\n");
	}
	if (ms->exported)
	{
		clean_arr(&(ms->exported));
		//ft_printf(2, "Freeing exported...\n");
	}
}
