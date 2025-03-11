#include "../../include/minishell.h"

void	pipe_process(int prev_pipe, int next_pipe)
{
	if (prev_pipe > 0)
	{
		if (dup2(prev_pipe, STDIN_FILENO) == -1) //It duplicates previous pipes read-end to stadard input
		{
			close(prev_pipe);
			exit(1);
		}	
	}
	if (next_pipe > 0)
	{
		if (dup2(next_pipe, STDOUT_FILENO) == -1) //It duplicates the next pipes write-end to standard output
		{
			close(next_pipe);
			exit(1);
		}	
	}
}

void    setup_pipes(int *pipe_fd, int i, int num_cmds, int cur_fd)
{
    if (i == 0)
        pipe_process(0, pipe_fd[1]);
    else if (i == num_cmds - 1)
        pipe_process(cur_fd, 0);
    else
        pipe_process(cur_fd, pipe_fd[1]);
}