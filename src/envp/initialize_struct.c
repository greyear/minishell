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
t_ms	*initialize_struct(char **envp)
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
	return (ms);
}

/**
 * @brief Frees all allocated memory in ms structure.
 * 
 * This function cleans up and deallocates all dynamically allocated resources 
 * within the `t_ms` structure, including environment variables, token lists, 
 * blocks, and other. Finally, it frees the `t_ms` structure itself.
 * 
 * @param ms A pointer to the `t_ms` structure to be cleaned. If `ms` is NULL, 
 *        the function does nothing.
 */
void	clean_struct(t_ms *ms)
{
	if (!ms)
		return ;
	if (ms->envp)
		clean_arr(&(ms->envp));
	if (ms->exported)
		clean_arr(&(ms->exported));
	if (ms->tokens)
		clean_token_list(&(ms->tokens));
	if (ms->blocks)
		clean_block_list(&(ms->blocks));
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
	}
	if (ms->exported)
	{
		clean_arr(&(ms->exported));
	}
}
