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

/*void	handle_heredoc(t_ms *ms, char *limiter)
{
	char	*line;
	int		temp_fd;
	char	*filename;

	// Generate unique filename
	filename = generate_heredoc_filename(ms->heredoc_count);
	ms->heredoc_files[ms->heredoc_count++] = filename;

	// Open heredoc file
	temp_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (temp_fd < 0)
	{
		perror("heredoc: open failed");
		exit(1);
	}
	while (1)
	{
		write(STDOUT_FILENO, "heredoc> ", 9);
		line = get_next_line(STDIN_FILENO);
		if (!line)
		{
			perror("heredoc: read error");
			close(temp_fd);
			exit(1);
		}
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0 && line[ft_strlen(limiter)] == '\n')
		{
			free(line);
			break;
		}
		write(temp_fd, line, ft_strlen(line));
		free(line);
	}
	close(temp_fd);
	temp_fd = open(filename, O_RDONLY);
	if (temp_fd < 0)
	{
		perror("heredoc: reopen failed");
		exit(1);
	}
	// Redirect STDIN to read from the heredoc file
	dup2(temp_fd, STDIN_FILENO);
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
		cmd->infile = handle_heredoc(ms, token->file);
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
