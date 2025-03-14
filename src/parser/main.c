
#include "../../include/minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <bits/types.h>
#include <bits/types.h>
//#include <asm-generic/termbits.h> //delete for school computers

volatile sig_atomic_t	g_sgnl;
volatile sig_atomic_t	g_sgnl;

void print_tokens(t_token *token_list)
{
	t_token *cur = token_list;

	printf("Tokens:\n");
	while (cur)
	{
		printf("Type: %d, Data: %s, Quotes: %c, Redir: %d, Ambig: %d, File: %s\n", cur->type, cur->data, cur->quote, cur->specific_redir, cur->ambiguous, cur->file);
		cur = cur->next;
	}
}
/*
static void print_blocks(t_block *block_list)
{
	t_block *cur = block_list;

	printf("\nBlocks:\n");
	while (cur)
	{
		printf("Block Start: %s (Type: %d) | Block End: %s (Type: %d)\n",
		cur->start ? cur->start->data : "NULL",
		cur->start ? cur->start->type : (t_token_type)-1,  // <-- приведение типа
		cur->end ? cur->end->data : "NULL",
		cur->end ? cur->end->type : (t_token_type)-1);  // <-- приведение типа

		cur = cur->next;
	}
}

*//*static void print_cmds(t_cmd *cmd_list)
{
	t_cmd *cur = cmd_list;

	printf("\nCommands:\n");
	while (cur)
	{
		printf("\nCommand %d:\n", cur->num);
		printf("  Name: %s\n", cur->name ? cur->name : "NULL");
		printf("Infile %d:\n", cur->infile);
		printf("Outfile %d:\n", cur->outfile);

		if (cur->args)
		{
			printf("  Args: ");
			for (int i = 0; cur->args[i]; i++)
				printf("\"%s\" ", cur->args[i]);
			printf("\n");
		}
		else
			printf("  Args: NULL\n");

		cur = cur->next;
	}
}*/

/*
static void	input_output(t_cmd *cmd)
{
	t_cmd	*cur;

	cur = cmd;
	while (cur)
	{
		if (cur->infile != DEF && cur->infile != NO_FD)
		{
			dup2(cur->infile, STDIN_FILENO);
		}
		if (cur->outfile != DEF && cur->outfile != NO_FD)
		{
			dup2(cur->outfile, STDOUT_FILENO);
		}
		cur = cur->next;
	}
}*/

static void	cleanup_after_execution(t_ms *ms)
{
	close(ms->saved_stdin);
	close(ms->saved_stdout);
	if (ms->heredoc_files)
		cleanup_heredocs(ms->heredoc_files);
	reset_heredocs(ms);
	clean_token_list(&(ms->tokens));
	clean_block_list(&(ms->blocks));
	clean_cmd_list(&(ms->cmds));
}

static void	execute_commands(t_ms *ms)
{
	t_cmd	*cur;
	int		i;
	
	cur = ms->cmds;
	i = 0;
	while (cur)
	{
		cur = cur->next;
		i++;
	}
	if (is_builtin(ms->cmds) && if_children_needed(ms->cmds) == false && i == 1)
		handle_builtin(ms->cmds, ms, 0);
	else
	{
		if (i == 1)
			make_one_child(ms->cmds, ms);
		else
			make_multiple_childs(i, ms->cmds, ms);
	}
	close_fds(ms->cmds);
}

static void	malloc_heredocs(t_ms *ms, t_token *token)
{
	t_token	*cur;
	int		heredoc_count;

	cur = token;
	heredoc_count = 0;
	while (cur)
	{
		if (cur->type == HEREDOC)
			heredoc_count++;
		cur = cur->next;
	}
	ms->heredoc_files = malloc(sizeof(char *) * (heredoc_count + 1)); // Support 100 heredocs max
	if (!ms->heredoc_files)
	{
		perror("heredoc: memory allocation failed");
		clean_struct(ms);
		exit(1);
	}
	ft_memset(ms->heredoc_files, 0, sizeof(char *) * (heredoc_count + 1)); // Set all entries to NULL
}

static void	inout(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
}

static int	tokenize_input(char **input, t_ms *ms)
{
	ms->tokens = tokenization(*input, ms);
	free(*input);
	if (!ms->tokens)
	{
		ft_putstr_fd("Error: tokenization failed\n", STDERR_FILENO);
		return (0);
	}
	malloc_heredocs(ms, ms->tokens);
	if (!ms->heredoc_files)
	{
		perror("heredoc memory allocaton failed");
		clean_token_list(&(ms->tokens));
		return (0);
	}
	put_files_for_redirections(ms->tokens);
	return (1);
}

static int	create_blocks_and_cmds_lists(t_ms *ms)
{
	int		err_syntax;

	err_syntax = 0;
	ms->blocks = create_blocks_list(ms->tokens, NULL, &err_syntax);
	if (err_syntax)
	{
		ft_putstr_fd("Error: failed to create blocks\n", STDERR_FILENO);
		clean_token_list(&(ms->tokens));
		return (0);
	}
	ms->cmds = create_cmd_list(ms->blocks, ms);
	if (!ms->cmds)
	{
		ft_putstr_fd("Error: failed to create commands\n", STDERR_FILENO);
		clean_token_list(&(ms->tokens));
		clean_block_list(&(ms->blocks));
		return (0);
	}
	return (1);
}

static int	process_input(char **input, t_ms *ms)
{
	int		err_syntax;

	err_syntax = 0;
	if ((*input)[0] == '\0') // Ignore empty input (Enter)
	{
		free(*input);
		if (g_sgnl == SIGINT)
		{
			ms->exit_status = 130;
			g_sgnl = 0;
		}
		if (g_sgnl == SIGINT)
		{
			ms->exit_status = 130;
			g_sgnl = 0;
		}
		return (0);
	}
	if (g_sgnl == SIGINT)
	{
		ms->exit_status = 130;
		g_sgnl = 0;
	}
	if (g_sgnl == SIGINT)
	{
		ms->exit_status = 130;
		g_sgnl = 0;
	}
	err_syntax = validate_input(*input);
	if (err_syntax)
	{
		history_exit(ms);
		clean_struct(ms);
		free(*input);
		exit(err_syntax);
	}
	add_line_to_history(*input, ms);
	return (1);
}
int	init_terminal_signals(void)
{
	struct termios	term;

	if (isatty(STDIN_FILENO))
	{
		if (tcgetattr(STDIN_FILENO, &term) == -1)
		{
			perror("tcgetattr failed");
			return (1);
		}
		term.c_lflag &= ~ECHOCTL;
		if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
		{
			perror("tcsetattr failed");
			return (1);
		}
	}
	//signals
	return (0);
}

int main(int argc, char **argv, char **envp)
{
	t_ms	*ms;
	char	*input;
	int		exit_code;

	if (argc != 1 && argv)
	{
		ft_putstr_fd("Usage: ./minishell\n", STDERR_FILENO);
		return (1);
	}
	init_terminal_signals();
	init_terminal_signals();
	ms = initialize_struct(envp);
	while (1)
	{
		// Reading the input
		inout(ms->saved_stdin, ms->saved_stdout); // Restore STDIN and STDOUT
		// FOR USUAL EXECUTION
		signal_mode(INTERACTIVE);
		input = readline("minishell> ");
		signal_mode(IGNORE);
		//FOR TESTER
		/*if (isatty(fileno(stdin))) // If running interactively
			input = readline("minishell> ");
		else // If receiving input from another program
		{
			char *line = get_next_line(fileno(stdin));
			if (!line) // Handle EOF (Ctrl+D in non-interactive mode)
				break;
			input = ft_strtrim(line, "\n"); // Remove newline from input
			free(line);
		}*/
		if (!input) // EOF check (Ctrl+D)
		{
			printf("exit\n");
			break;
		}
		if (!process_input(&input, ms))
			continue; 
		if (!tokenize_input(&input, ms))
			continue;
		if (!create_blocks_and_cmds_lists(ms))
			continue;
		execute_commands(ms);
		cleanup_after_execution(ms);
	}
	clean_cmd_list(&(ms->cmds));
	history_exit(ms);
	exit_code = ms->exit_status;
	clean_struct(ms);
	rl_clear_history();
	return (exit_code);
}
