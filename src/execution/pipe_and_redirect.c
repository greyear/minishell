/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_and_redirect.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 13:57:44 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 13:57:46 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
 * @param prev_pipe The file descriptor of the previous pipe's read end, 
 *                  or -1 if no previous pipe.
 * @param next_pipe The file descriptor of the next pipe's write end, or -1 
 *                  if no next pipe.
 * 
 * @return This function does not return; it exits the process if an error 
 *         occurs while duplicating the file descriptors.
 */
void	pipe_process(int prev_pipe, int next_pipe)
{
	if (prev_pipe > 0)
	{
		if (dup2(prev_pipe, STDIN_FILENO) == -1)
		{
			close(prev_pipe);
			exit(1);
		}	
	}
	if (next_pipe > 0)
	{
		if (dup2(next_pipe, STDOUT_FILENO) == -1)
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

/**
 * @brief Redirects the input and output of the current process based 
 *        on the provided file descriptors.
 *
 * This function handles the redirection of input (stdin) and output 
 * (stdout) for the current process using the `dup2` system call. It 
 * checks the given file descriptors (`infile` and `outfile`) and, if 
 * they are valid, redirects stdin and/or stdout accordingly. If either 
 * file descriptor is invalid, the function closes the files and exits 
 * with an error status. In case of failure during `dup2`, it sets the 
 * shell's exit status to indicate a system error.
 *
 * @param infile The file descriptor for input redirection.
 * @param outfile The file descriptor for output redirection.
 * @param ms The minishell structure containing the exit status and 
 *           other relevant state.
 */
void	redirect_process(int infile, int outfile, t_ms *ms)
{
	if (infile == NO_FD || outfile == NO_FD)
	{
		close_file(infile);
		close_file(outfile);
		exit(1);
	}
	if (infile != DEF)
	{
		if (dup2(infile, STDIN_FILENO) == -1)
		{
			ms->exit_status = SYSTEM_ERR;
			return ;
		}
	}
	if (outfile != DEF)
	{
		if (dup2(outfile, STDOUT_FILENO) == -1)
			ms->exit_status = SYSTEM_ERR;
	}
}
