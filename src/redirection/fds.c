/*
HANDLNG REDIRECTIONS:

HEREDOC/<< :
Works like this: << LIMITER (writes to temp_fd until LIMITER is written to terminal, then temp_fd becomes STDIN)
Function that handles this: handle_heredoc(char *limiter) takes the limiter as parameter
stops writing to file when the LIMITER is written to terminal

< :
Works like this: < infile (infile BECOMES STDIN)
Function that handles this: redirection_infile(char *file)
takes the file that we want to change to STDIN as parameter

> :
Works like this: > outfile (outfile becomes STDOUT)
Function that handles this: redirection_outfile_emptied(char *file)
Takes the file that we want to change to STDIN as parameter. 
Empties the file if it exists, else creates one with correct permissions

>> : 
Works like this: >> outfile (outfile becomes STDOUT)
Function that handles this: redirection_outfile_append(char *file)
Takes the file that we want to change to STDIN as parameter. 
Writes to the endoffile,if file does not exist, creates one with correct permissions

****FOR EXAMPLE****
> infile cat | "ls -la" | "grep error" >> outfile

BASICALLY IDEA IS (what i think we should maybe do, if im correct)
1. FIRST CALL FOR REDIRECTION_INFILE() WITH INFILE
2. SECOND CALL REDIRECTION_OUTFILE_APPEND() WITH INFILE
3. THEN CALL FOR FT_PIPE() WITH (3=cmd_amount, CMD_ARRAY, ENVP)
CMD_ARRAY[0] = "CAT", CMD_ARRAY[1] = "LS -LA", CMD_ARRAY[2] = "GREP ERROR", CMD_ARRAY[3] = NULL;

*/

//#include "seela.h"
#include "../../include/minishell.h"

/*
void handle_heredoc(char *limiter, t_ms *ms)
{
	char    buffer[1024];
	size_t  len;
	ssize_t bytes_read;
	int     temp_fd;

	len = 0;
	temp_fd = open(".heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (temp_fd < 0)
		exit(1); //I guess we cannot just exit without an error msg and cleaning?
	while (1)
	{
		write(STDOUT_FILENO, "heredoc> ", 9);
		len = 0;
		bytes_read = read(STDIN_FILENO, buffer + len, 1);
		while (bytes_read > 0)
		{
			if (buffer[len] == '\n')
			{
				buffer[len] = '\0';
				break;
			}
			len++;
			bytes_read = read(STDIN_FILENO, buffer + len, 1);
		}
		if (bytes_read < 0)
			exit(1);
		if (ft_strcmp(buffer, limiter) == 0)
			break;
		write(temp_fd, buffer, len);
		write(temp_fd, "\n", 1);
	}
	close(temp_fd);
}*/

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
/*
void	redirection_outfile_append(char *file)
{
	int		file2;

	file2 = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (file2 < 0)
		exit(1);
	check_access(file, 0);
	if (dup2(file2, STDOUT_FILENO) == -1)
	{
		close(file2);
		exit(1);
	}
	close(file2);
}*/

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
		dup2(cmd->outfile, STDOUT_FILENO);
		close (cmd->outfile);
	}
}

/*
void	redirection_infile(char *file)
{
	int		file1;

	check_access(file, 1);	
	file1 = open(file, O_RDONLY);
	if (file1 < 0)
		exit(1);
	if (dup2(file1, STDIN_FILENO) == -1)
	{
		close(file1);
		exit(1);
	}
	close(file1);
}*/

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
		//printf("infile %s\n", token->file);
		cmd->infile = open(token->file, O_RDONLY);
		//printf("infile fd %d\n", cmd->infile);
		if (cmd->infile < 0)
			check_access(token->file, RD);
		dup2(cmd->infile, STDIN_FILENO);
		close (cmd->infile);
	}
}
/*
void	redirection_outfile_emptied(char *file)
{
	int		file2;

	file2 = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file2 < 0)
		exit(1);
	check_access(file, 0);
	if (dup2(file2, STDOUT_FILENO) == -1)
	{
		close(file2);
		exit(1);
	}
	close(file2);
}*/



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