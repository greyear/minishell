/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaners_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azinchen <azinchen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 19:53:51 by azinchen          #+#    #+#             */
/*   Updated: 2024/12/18 19:53:56 by azinchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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
