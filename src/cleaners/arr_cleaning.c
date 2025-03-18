#include "../../include/minishell.h"

void	clean_arr(char ***arr)
{
	int	i;

	if (!arr || !*arr)
		return;
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
	int	i;
	
	i = 0;
	if (!filenames)
		return;
	while (filenames[i])
	{
		if (unlink(filenames[i]) == -1)
		{
			perror("unlink fail");
			exit(1);
		}
		free(filenames[i]);
		i++;
	}
	if (filenames)
		free(filenames);
}

void	free_pids(t_pipe *p)
{
	if (p->pids)
	{
		free(p->pids);
		p->pids = NULL;
	}
}