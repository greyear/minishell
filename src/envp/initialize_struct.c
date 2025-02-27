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
	return (ms);
}

void	clean_struct(t_ms *ms)
{
	if (!ms)
		return ;
	if (ms->envp)
		clean_arr(&(ms->envp));
	if (ms->exported)
		clean_arr(&(ms->exported));
	/*if (ms->tokens)
		clean_token_list(&(ms->tokens));
	if (ms->blocks)
		clean_block_list(&(ms->blocks));*/
	/*if (ms->cmds)
		clean_cmd_list(&(ms->cmds));*/
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
