#include "../../include/minishell.h"

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
		cleanup_heredocs(ms->heredoc_files);
	free(ms);
 }
 
 /*void	clean_struct_fields(t_ms *ms)
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
 }*/
