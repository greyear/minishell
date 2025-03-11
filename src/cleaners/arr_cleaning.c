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

void	cleanup_heredocs(char **filenames)
{
    int		i;
	
	i = 0;
    while (filenames[i])
	{
        if (unlink(filenames[i]) == -1)
		{
			perror("unlink fail\n");
			exit(1);
		}
		free(filenames[i]);
        i++;
    }
    free(filenames);
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
	if (p->pids)
	{
		free(p->pids);
		p->pids = NULL;
	}
    cleanup_heredocs(p->ms->heredoc_files);
    p->ms->heredoc_files = malloc(sizeof(char *) * 100);
    ft_memset(p->ms->heredoc_files, 0, sizeof(char *) * 100);
    p->ms->heredoc_count = 0;
}