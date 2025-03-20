#include "../../include/minishell.h"

/**
 * @brief Cleans up temporary heredoc files by unlinking them and freeing 
 *        the associated memory.
 *
 * This function iterates over the `filenames` array, unlinking each file 
 * (removing it from the filesystem). If unlinking fails, it prints an error, 
 * sets the exit status to indicate a system error, and stops further unlinking. 
 * After all files are processed or an error occurs, the function frees the 
 * memory associated with the `filenames` array.
 *
 * @param filenames An array of filenames representing temporary heredoc 
 *                  files to be cleaned up.
 * @param ms The shell structure containing the exit status to be updated 
 *           if an error occurs.
 */

void	cleanup_heredocs(char **filenames, t_ms *ms)
{
	int		i;

	i = 0;
	if (!filenames)
		return ;
	while (filenames[i])
	{
		if (unlink(filenames[i]) == -1)
		{
			perror("unlink fail");
			ms->exit_status = SYSTEM_ERR;
			break ;
		}
		i++;
	}
	clean_arr(&filenames);
}

/**
 * @brief Frees the allocated memory for process IDs in a pipeline structure.
 * 
 * This function checks if the `p->pids` array exists and frees it to prevent 
 * memory leaks. It then sets the pointer to `NULL` to avoid accidental 
 * access to freed memory.
 * 
 * @param p A pointer to the `t_pipe` structure containing the process ID 
 *          array.
 */

void	free_pids(t_pipe *p)
{
	if (p->pids)
	{
		free(p->pids);
		p->pids = NULL;
	}
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
	if (ms->heredoc_files)
		clean_arr(&(ms->heredoc_files));
	if (ms->tokens)
		clean_token_list(&(ms->tokens));
	if (ms->blocks)
		clean_block_list(&(ms->blocks));
	if (ms->heredoc_files)
		cleanup_heredocs(ms->heredoc_files, ms);
	free(ms);
}

/**
 * @brief Cleans up allocated memory and temporary files after command execution.
 * 
 * This function performs cleanup operations after executing a command or 
 * pipeline. It ensures that temporary files, token lists, command structures, 
 * and any allocated resources related to heredocs are properly freed.
 * 
 * @param ms The main shell structure containing environment data and execution 
 *           context.
 */

void	cleanup_after_execution(t_ms *ms)
{
	if (ms->heredoc_files)
		cleanup_heredocs(ms->heredoc_files, ms);
	reset_heredocs(ms);
	clean_token_list(&(ms->tokens));
	clean_block_list(&(ms->blocks));
	clean_cmd_list(&(ms->cmds));
}
