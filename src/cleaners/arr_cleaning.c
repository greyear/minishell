
#include "../../include/minishell.h"

void	clean_arr(char ***arr)
{
	int	i;

	if (!arr || !*arr)
		return ;
	i = 0;
	while ((*arr)[i])
	{
		free((*arr)[i]);
		(*arr)[i] = NULL;
		i++;
	}
	free(*arr);
	*arr = NULL;
}

void	free_int_array(int **array)
{
	int	i;	

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void cleanup_heredocs(char **filenames)
{
    int	i;
	
	i = 0;
    while (filenames[i])
	{
        if (unlink(filenames[i]) == -1)  // Remove file
		{
			perror("unlink fail\n");
			return;
		}
		free(filenames[i]);    // Free the memory for the filename
        i++;
    }
    free(filenames);  // Free the array of filenames
}

void	close_fds(t_cmd *cmd)
{
	t_cmd	*cur;

	cur = cmd;
	while (cur)
	{
		if (cur->infile != DEF && cur->infile != NO_FD)
			close(cur->infile);
		if (cur->outfile != DEF && cur->outfile != NO_FD)
			close(cur->outfile);
		cur = cur->next;
	}
}

void	cleanup_after_execution(t_pipe *p)
{
	//close_pipes(p->num_cmds, p->fd);
    wait_for_children(p->num_cmds, p->last_pid, p->ms);
    cleanup_heredocs(p->ms->heredoc_files);
    p->ms->heredoc_files = malloc(sizeof(char *) * 100);
    ft_memset(p->ms->heredoc_files, 0, sizeof(char *) * 100);
    p->ms->heredoc_count = 0;
}

void    close_pipes(int num_cmds, int **pipe_fd)
{
    int     i;

    i = 0;
    while (i < num_cmds - 1)
    {
        if (pipe_fd[i])
        {
            close(pipe_fd[i][0]);
            close(pipe_fd[i][1]);
            free(pipe_fd[i]);
        }
        i++;
    }
    if (pipe_fd)
        free(pipe_fd);
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
		cleanup_heredocs(ms->heredoc_files);
	if (ms->saved_stdin != NO_FD)
		close(ms->saved_stdin);
	if (ms->saved_stdout != NO_FD)
		close(ms->saved_stdout);
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