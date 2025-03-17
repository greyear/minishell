#include "../../include/minishell.h"

/**
 * @brief Redirects the input and output for the process using pipes.
 * 
 * This function sets up the necessary file descriptors for the process by 
 * duplicating the given pipe file descriptors to the standard input (STDIN) 
 * and standard output (STDOUT) file descriptors. It ensures that data flows
 * correctly between processes in a pipeline.
 * 
 * If a valid `prev_pipe` is provided, its read end is duplicated to STDIN.
 * If a valid `next_pipe` is provided, its write end is duplicated to STDOUT.
 * 
 * @param prev_pipe The file descriptor of the previous pipe's read end, or -1 if no previous pipe.
 * @param next_pipe The file descriptor of the next pipe's write end, or -1 if no next pipe.
 * 
 * @return This function does not return; it exits the process if an error occurs while duplicating the file descriptors.
 */

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

/**
 * @brief Configures the pipe file descriptors based on the command position.
 * 
 * This function sets up the appropriate pipes for each command in a pipeline.
 * It handles the input and output file descriptors based on the command's
 * position in the pipeline. If the command is the first or last, it adjusts
 * the pipe file descriptors accordingly.
 * 
 * @param pipe_fd The file descriptors for the pipe.
 * @param i The index of the current command in the pipeline.
 * @param num_cmds The total number of commands in the pipeline.
 * @param cur_fd The current file descriptor for input redirection.
 */

void	setup_pipes(int *pipe_fd, int i, int num_cmds, int cur_fd)
{
	if (i == 0)
		pipe_process(0, pipe_fd[1]);
	else if (i == num_cmds - 1)
		pipe_process(cur_fd, 0);
	else
		pipe_process(cur_fd, pipe_fd[1]);
}