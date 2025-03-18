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

void	close_all_fds(t_pipe *p)
{
	close_file(p->fd[0]);
	close_file(p->fd[1]);
	close(p->cur_fd);
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