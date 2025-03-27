/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fds.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 14:22:54 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 14:22:56 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Checks file accessibility for reading or writing.
 * 
 * This function verifies whether the specified file exists and whether 
 * the necessary permissions are granted for the requested operation 
 * (read or write). If the file does not exist or the required 
 * permissions are missing, an appropriate error message is printed.
 * 
 * @param filename The name of the file to check.
 * @param operation The operation type (RD for reading, WR for writing).
 */
void	check_access(char *filename, t_oper operation)
{
	if (operation == RD)
	{
		if (access(filename, F_OK) == -1)
			print_file_error(filename, NO_FILE);
		else if (access(filename, R_OK) == -1)
			print_file_error(filename, PERM_DEN);
	}
	else
	{
		if (access(filename, F_OK) == -1)
			print_file_error(filename, NO_FILE);
		else if (access(filename, W_OK) == -1)
			print_file_error(filename, PERM_DEN);
		else
		{
			ft_putstr_fd(OWN_ERR_MSG, STDERR_FILENO);
			ft_putstr_fd(filename, STDERR_FILENO);
			ft_putstr_fd(IS_DIR_ERR, STDERR_FILENO);
		}
	}
}

/**
 * @brief Sets up the input file descriptor for a heredoc.
 * 
 * This function handles input redirection from a heredoc by processing 
 * the specified delimiter. If an input file descriptor is already open, 
 * it is closed first. In case of an ambiguous redirection, an error is 
 * printed, and the input descriptor is set to an invalid state. Otherwise, 
 * the heredoc content is processed and assigned to the command's input.
 * 
 * @param token A pointer to the token containing heredoc information.
 * @param cmd A pointer to the command structure where the input 
 * descriptor is stored.
 * @param ms A pointer to the shell structure.
 */
void	put_heredoc_fd(t_token *token, t_cmd *cmd, t_ms *ms)
{
	if (cmd->infile > 0)
		close(cmd->infile);
	if (token->ambiguous)
	{
		cmd->infile = NO_FD;
		print_file_error(token->file, AMBIG);
	}
	else
	{
		cmd->infile = handle_heredoc(ms, token->file, token);
	}
}

/**
 * @brief Opens and sets the output file descriptor for a command.
 * 
 * This function manages the output redirection for a command. If an 
 * output file is already open, it is closed first. Then, based on the 
 * token type, the function either truncates or appends to the specified 
 * file. In case of an ambiguous redirection, an error is printed, and 
 * the output descriptor is set to an invalid state.
 * 
 * @param token A pointer to the token containing file information.
 * @param cmd A pointer to the command structure where the output 
 * descriptor is stored.
 */
void	put_outfile_fd(t_token *token, t_cmd *cmd)
{
	char	*file;

	file = token->file;
	if (cmd->outfile > 0)
		close(cmd->outfile);
	if (token->ambiguous)
	{
		cmd->outfile = NO_FD;
		print_file_error(token->file, AMBIG);
	}
	else
	{
		if (token->type == OUT)
			cmd->outfile = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (token->type == APPEND)
			cmd->outfile = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (cmd->outfile < 0)
			check_access(file, WR);
	}
}

/**
 * @brief Opens and sets the input file descriptor for a command.
 * 
 * This function handles input redirection by opening the specified file 
 * in read-only mode. If an input file descriptor is already open, it is 
 * closed first. In case of an ambiguous redirection, an error is printed, 
 * and the input descriptor is set to an invalid state.
 * 
 * @param token A pointer to the token containing file information.
 * @param cmd A pointer to the command structure where the input 
 * descriptor is stored.
 */
void	put_infile_fd(t_token *token, t_cmd *cmd)
{
	if (cmd->infile > 0)
		close(cmd->infile);
	if (token->ambiguous)
	{
		cmd->infile = NO_FD;
		print_file_error(token->file, AMBIG);
	}
	else
	{
		cmd->infile = open(token->file, O_RDONLY);
		if (cmd->infile < 0)
			check_access(token->file, RD);
	}
}
