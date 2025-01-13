/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azinchen <azinchen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 20:34:56 by azinchen          #+#    #+#             */
/*   Updated: 2024/12/18 20:35:00 by azinchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	child(t_pipex *p)
{
	int	fd_out;

	if (p->cmd_num == 0)
		p->cur_fd = open_infile(p);
	if (dup2(p->cur_fd, STDIN_FILENO) == -1)
		error_clean_exit_code(ERR_DUP2, EXIT_FAILURE, &p);
	if (p->cmd_num == 0)
		close(p->cur_fd);
	if (p->cmd_num == p->argc - 4)
	{
		fd_out = open_outfile(p);
		if (dup2(fd_out, STDOUT_FILENO) == -1)
			error_clean_exit_code(ERR_DUP2, EXIT_FAILURE, &p);
		close(fd_out);
		close_fds(p->cur_fd, p->fd[1]);
	}
	else
	{
		if (dup2(p->fd[1], STDOUT_FILENO) == -1)
			error_clean_exit_code(ERR_DUP2, EXIT_FAILURE, &p);
		close_fds(p->cur_fd, p->fd[1]);
	}
	close(p->fd[0]);
	handle_command(p->argv[p->cmd_num + 2], p);
	exit(EXIT_SUCCESS);
}

static void	pipex(t_pipex *p)
{
	while (p->cmd_num < p->argc - 3)
	{
		if (pipe(p->fd) == -1)
			error_clean_exit_code(ERR_PIPE, EXIT_FAILURE, &p);
		p->pids[p->cmd_num] = fork();
		if (p->pids[p->cmd_num] < 0)
			error_clean_exit_code(ERR_FORK, EXIT_FAILURE, &p);
		if (p->pids[p->cmd_num] == 0)
			child(p);
		close_fds(p->cur_fd, p->fd[1]);
		p->cur_fd = p->fd[0];
		p->cmd_num++;
	}
	if (close(p->cur_fd) == -1)
		error_clean_exit_code(ERR_CLOSE, EXIT_FAILURE, &p);
}

static int	waiting(t_pipex *p)
{
	int	status;
	int	exit_code;
	int	pid_counter;

	pipex(p);
	pid_counter = 0;
	while (pid_counter < p->cmd_num)
	{
		if (wait(&status) == p->pids[p->cmd_num - 1])
			exit_code = status;
		pid_counter++;
	}
	return ((exit_code >> 8) & 255);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_pipex	*p;
	int		status;

	if (argc < 5)
		args_number_error();
	p = (t_pipex *)malloc(sizeof(t_pipex));
	if (!p)
		error_exit_code(ERR_MALLOC, EXIT_FAILURE);
	p->argc = argc;
	p->argv = argv;
	p->envp = envp;
	init_p(p);
	p->pids = (pid_t *)malloc((p->argc - 3) * sizeof(pid_t));
	if (!p->pids)
		error_clean_exit_code(ERR_MALLOC, EXIT_FAILURE, &p);
	status = waiting(p);
	clean_struct(&p);
	return (status);
}
