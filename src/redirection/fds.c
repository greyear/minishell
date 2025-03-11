#include "../../include/minishell.h"

/*static int	check_if_dir(char *path)
{
	DIR *dir;
	
	dir = opendir(path);
	if (dir)
	{
		closedir(dir);
		return (1);
	}
	return (0);
}*/

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

	/*if (check_if_dir(token->file))
	{
		cmd->outfile = NO_FD;
		print_file_error(token->file, DIRECT);
		return ;
	}*/

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
 * @brief Redirects input and output streams for a process.
 * 
 * This function duplicates the file descriptors for input and output. If the `infile` is not set to `NO_FD` or
 * `DEF`, it redirects the standard input (`STDIN_FILENO`) to the file descriptor specified by `infile`. Similarly, if
 * the `outfile` is not `NO_FD` or `DEF`, it redirects the standard output (`STDOUT_FILENO`) to the file descriptor
 * specified by `outfile`. If any `dup2` operation fails, the corresponding file descriptor is closed, and the program
 * exits with a status of `1`.
 * 
 * @param infile The input file descriptor to be redirected to `STDIN_FILENO`. If it is `NO_FD` or `DEF`, no redirection occurs.
 * @param outfile The output file descriptor to be redirected to `STDOUT_FILENO`. If it is `NO_FD` or `DEF`, no redirection occurs.
 * 
 * @return This function does not return; it modifies the process's input and output streams based on the provided file descriptors.
 */

void	redirect_process(int infile, int outfile)
{
	if (infile != NO_FD && infile != DEF)
	{
		if (dup2(infile, STDIN_FILENO) == -1) //It duplicates previous pipes read-end to stadard input
		{
			close(infile);
			exit(1);
		}
		close(infile);
	}
	if (outfile != NO_FD && outfile != DEF)
	{
		if (dup2(outfile, STDOUT_FILENO) == -1) //It duplicates the next pipes write-end to standard output
		{
			close (outfile);
			exit(1);
		}
		close(outfile);
	}
}
