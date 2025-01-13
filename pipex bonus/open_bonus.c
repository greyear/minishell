/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azinchen <azinchen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 20:11:43 by azinchen          #+#    #+#             */
/*   Updated: 2024/12/18 20:11:47 by azinchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	open_infile(t_pipex *p)
{
	int	fd_in;

	check_first_file(p);
	fd_in = open(p->argv[1], O_RDONLY);
	if (fd_in < 0)
	{
		close_fds(p->fd[0], p->fd[1]);
		error_clean_exit_code(ERR_OPEN, EXIT_FAILURE, &p);
	}
	return (fd_in);
}

int	open_outfile(t_pipex *p)
{
	int	fd_out;

	check_second_file(p);
	fd_out = open(p->argv[p->argc - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd_out < 0)
	{
		close_fds(p->fd[0], p->fd[1]);
		if (close(p->cur_fd) == -1)
			error_clean_exit_code(ERR_CLOSE, EXIT_FAILURE, &p);
		error_clean_exit_code(ERR_OPEN, EXIT_FAILURE, &p);
	}
	return (fd_out);
}
