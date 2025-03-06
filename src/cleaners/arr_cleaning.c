
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
		{
			close(cur->infile);
		}
		if (cur->outfile != DEF && cur->outfile != NO_FD)
		{
			close(cur->outfile);
		}
		cur = cur->next;
	}
}