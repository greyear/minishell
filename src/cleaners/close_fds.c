#include "../../include/minishell.h"

void	close_fds2(int fd1, int fd2)
{
	if (fd1 != -1)
		close(fd1);
	if (fd2 != -1)
		close(fd2);
}

void	close_file(int file)
{
	if (file != -1)
		close(file);
}

void	close_all_fds(t_pipe *p, t_ms *ms)
{
	close_file(p->fd[0]);
	close_file(p->fd[1]);
	close(p->cur_fd);
	close(ms->saved_stdin);
	close(ms->saved_stdout);
}