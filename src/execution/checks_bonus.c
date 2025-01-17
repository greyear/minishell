
#include "../../include/minishell.h"

void	check_first_file(t_pipex *p)
{
	if (access(p->argv[1], F_OK) == -1)
	{
		close_fds(p->fd[0], p->fd[1]);
		cmd_error(ZSH_NO_FILE, p->argv[1], 1, &p);
	}
	if (access(p->argv[1], R_OK) == -1)
	{
		close_fds(p->fd[0], p->fd[1]);
		cmd_error(PERM_DENIED, p->argv[1], 1, &p);
	}
}

void	check_second_file(t_pipex *p)
{
	if (access(p->argv[p->argc - 1], F_OK) == 0
		&& access(p->argv[p->argc - 1], W_OK) == -1)
	{
		close_fds(p->fd[0], p->fd[1]);
		close(p->cur_fd);
		cmd_error(PERM_DENIED, p->argv[p->argc - 1], 1, &p);
	}
}
