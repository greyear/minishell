/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_fds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 13:35:42 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 13:35:44 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Closes two file descriptors if they are valid.
 * 
 * This function checks if each file descriptor is valid (not equal to `-1`) 
 * and closes it. This helps prevent resource leaks by ensuring that unused 
 * file descriptors are properly closed.
 * 
 * @param fd1 The first file descriptor to close.
 * @param fd2 The second file descriptor to close.
 */
void	close_two_fds(int fd1, int fd2)
{
	close_file(fd1);
	close_file(fd2);
}

/**
 * @brief Closes a file descriptor if it is valid.
 * 
 * This function checks if the given file descriptor is not equal to `NO_FD`. 
 * If it is valid, the function closes it to free system resources.
 * 
 * @param file The file descriptor to be closed.
 */
void	close_file(int file)
{
	if (file != NO_FD && file != DEF)
		close(file);
}

/**
 * @brief Closes all file descriptors associated with a pipeline.
 * 
 * This function closes the input and output file descriptors stored in the 
 * `fd` array of the `t_pipe` structure, as well as the `cur_fd` descriptor.
 * It ensures that resources are properly released.
 * 
 * @param p A pointer to the `t_pipe` structure containing the file descriptors.
 */
void	close_pipe_fds(t_pipe *p)
{
	close_file(p->fd[0]);
	close_file(p->fd[1]);
	close_file(p->cur_fd);
}

/**
 * @brief Closes input and output file descriptors for a list of commands.
 * 
 * This function iterates through a linked list of commands, closing the input 
 * (`infile`) and output (`outfile`) file descriptors for each command. 
 * The function checks that the file descriptors are valid and not set to 
 * default values (`DEF`) or invalid values (`NO_FD`) before closing them.
 * 
 * @param cmd A pointer to the first command in the linked list of commands.
 */
void	close_every_cmds_fds(t_cmd *cmd)
{
	t_cmd	*cur;

	cur = cmd;
	while (cur)
	{
		close_file(cur->infile);
		close_file(cur->outfile);
		cur = cur->next;
	}
}
