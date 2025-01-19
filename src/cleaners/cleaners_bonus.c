
#include "../../include/minishell.h"

void	clean_arr(char ***arr)
{
	int	i;

	i = 0;
	if (*arr)
	{
		while ((*arr)[i])
		{
			free((*arr)[i]);
			i++;
		}
		free(*arr);
		*arr = NULL;
	}
}

void	clean_struct(t_pipex **p)
{
	if (!p || !*p)
		return ;
	if ((*p)->pids)
	{
		free((*p)->pids);
		(*p)->pids = NULL;
	}
	if ((*p)->cmds)
	{
		clean_arr(&((*p)->cmds));
		(*p)->cmds = NULL;
	}
	free(*p);
	*p = NULL;
}
