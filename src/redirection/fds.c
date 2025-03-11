#include "../../include/minishell.h"

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

/*
echo "Hello" > file1 > file2  # Только file2 получит "Hello"

< input.txt grep "word" > output.txt 
и
grep "word" < input.txt > output.txt
работают одинаково

cat < input.txt | grep "error" >> log.txt 2> errors.txt << EOF
error: something went wrong
all good
EOF
сработает

Редиректы выполняются слева направо, 
но heredoc (<<) всегда обрабатывается первым.

ambiguity:

$ echo hello > ${NONEXISTENT}: 
	bash: ${NONEXISTENT}: ambiguous redirect


*/
