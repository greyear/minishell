
#include "../../include/minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>


/*void print_tokens(t_token *token_list)
{
	t_token *cur = token_list;

	printf("Tokens:\n");
	while (cur)
	{
		printf("Type: %d, Data: %s, Quotes: %c, Redir: %d, Ambig: %d, File: %s\n", cur->type, cur->data, cur->quote, cur->specific_redir, cur->ambiguous, cur->file);
		cur = cur->next;
	}
}*/
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

static void	inout(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
}

int main(int argc, char **argv, char **envp)
{
	t_ms *ms;
	t_cmd	*cur;
	char *input;
	int err_syntax;
	int		i;

	// Args check
	if (argc != 1 && argv)
	{
		printf("Usage: ./minishell\n");
		return (1);
	}

	ms = initialize_struct(envp);
	check_shlvl(ms);
	if (!ms)
	{
		printf("Error: failed to initialize shell structure\n");
		return (1);
	}

	// Main loop
	while (1)
	{
		// Reading the input
		inout(ms->saved_stdin, ms->saved_stdout); // Restore STDIN and STDOUT
		
		// FOR USUAL EXECUTION
		input = readline("minishell> ");


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
		if (input[0] == '\0') // Ignore empty input (Enter)
		{
    		free(input);
    		continue;
		}

		// BNF checking
		err_syntax = validate_input(input);
		if (err_syntax)
		{
			history_exit(ms); //here?
			clean_struct(ms);
			free(input);
			exit(err_syntax);
		}

		add_line_to_history(input, ms); //fix
		/*if (ms->history[ms->history_num])
			printf("history %s in line %d\n", ms->history[ms->history_num], ms->history_num);*/

		// Parsing
		ms->tokens = tokenization(input, ms);
		free(input); // Освобождаем readline-буфер
		if (!ms->tokens)
		{
			printf("Error: tokenization failed\n");
			continue;
		}
		/*printf("after tokenization\n");
		print_tokens(ms->tokens);*/
		put_files_for_redirections(ms->tokens);
		ms->blocks = create_blocks_list(ms->tokens, NULL, &err_syntax);
		if (err_syntax)
		{
			printf("Error: failed to create blocks\n");
			clean_token_list(&(ms->tokens));
			continue;
		}
		ms->cmds = create_cmd_list(ms->blocks, ms);
		if (!ms->cmds)
		{
			printf("Error: failed to create commands\n");
			clean_token_list(&(ms->tokens));
			clean_block_list(&(ms->blocks));
			continue;
		}
		i = 0;
		cur = ms->cmds;
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
		clean_token_list(&(ms->tokens));
		clean_block_list(&(ms->blocks));
		clean_cmd_list(&(ms->cmds));
	}
	clean_cmd_list(&(ms->cmds));
	history_exit(ms); //here?
	int exit = ms->exit_status;
	clean_struct(ms);
	rl_clear_history();
	return (exit);
}
