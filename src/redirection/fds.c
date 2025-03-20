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
			ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
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
			cmd->outfile = open(token->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (token->type == APPEND)
			cmd->outfile = open(token->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (cmd->outfile < 0)
			check_access(token->file, WR);
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

/**
 * @brief Redirects input and output file descriptors for a process.
 * 
 * This function handles the redirection of standard input (STDIN) and standard 
 * output (STDOUT) for the process based on the provided file descriptors.
 * 
 * If the `infile` or `outfile` are valid (not equal to `NO_FD`), the function
 * duplicates the respective file descriptor to the corresponding standard input 
 * or output. It closes the original file descriptors after redirection to avoid 
 * resource leakage.
 * 
 * If either `infile` or `outfile` is set to `NO_FD`, the function closes both 
 * file descriptors and exits with an error code.
 * 
 * @param infile The file descriptor for standard input, or `NO_FD` to avoid redirection.
 * @param outfile The file descriptor for standard output, or `NO_FD` to avoid redirection.
 * 
 * @return This function does not return; it exits the process if an error occurs during redirection.
 */

void	redirect_process(int infile, int outfile)
{
	if (infile == NO_FD || outfile == NO_FD)
	{
		if (outfile != NO_FD)
			close(outfile);
		if (infile != NO_FD)
			close(infile);
		exit(1);
	}
	if (infile != DEF)
	{
		if (dup2(infile, STDIN_FILENO) == -1) //It duplicates previous pipes read-end to stadard input
		{
			close(infile);
			exit(1);
		}
		close(infile);
	}
	if (outfile != DEF)
	{
		if (dup2(outfile, STDOUT_FILENO) == -1) //It duplicates the next pipes write-end to standard output
		{
			close (outfile);
			exit(1);
		}
		close(outfile);
	}
}
