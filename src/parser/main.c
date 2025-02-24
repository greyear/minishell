
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
}

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

static void print_cmds(t_cmd *cmd_list)
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
	t_token *tokens;
	t_block *blocks;
	t_cmd *cmds;
	t_cmd	*cur;
	char *input;
	int err_flag;
	int	i;

	int saved_stdin = dup(STDIN_FILENO);
	int saved_stdout = dup(STDOUT_FILENO);

	// Args check
	if (argc != 1 && argv)
	{
		printf("Usage: ./minishell\n");
		return (1);
	}

	// t_ms init
	ms = initialize_struct(envp);
	if (!ms)
	{
		printf("Error: failed to initialize shell structure\n");
		return (1);
	}

	// Main loop
	while (1)
	{
		// Reading the input
		inout(saved_stdin, saved_stdout); // Restore STDIN and STDOUT
		input = readline("minishell> ");
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
		err_flag = validate_input(input);
		//printf("BNF validation result: %d\n", err_flag);
		if (err_flag)
		{
			printf("Error: invalid input format\n");
			free(input);
			continue;
		}

		if (*input)
			add_history(input);

		// Parsing
		tokens = tokenization(input, ms);
		free(input); // Освобождаем readline-буфер

		if (!tokens)
		{
			printf("Error: tokenization failed\n");
			continue;
		}

		//print_tokens(tokens);
		put_files_for_redirections(tokens);
		//printf("tokens again \n");
		//print_tokens(tokens);

		blocks = create_blocks_list(tokens, NULL, &err_flag);
		if (err_flag)
		{
			printf("Error: failed to create blocks\n");
			clean_token_list(&tokens);
			continue;
		}

		// Blocks printing
		//print_blocks(blocks);

		// Cmds creation
		cmds = create_cmd_list(blocks, ms);
		if (!cmds)
		{
			printf("Error: failed to create commands\n");
			clean_token_list(&tokens);
			clean_block_list(&blocks);
			continue;
		}
		//print_cmds(cmds);
		i = 0;
		cur = cmds;
		while (cur)
		{
			cur = cur->next;
			i++;
		}

		// Cmd printing
		//print_cmds(cmds);
		//input_output(cmds);
		if (is_builtin(cmds) && if_children_needed(cmds) == false && i == 1)
		{
			//printf("Here1\n");
			handle_builtin(cmds, ms, 0);
		}
		else
		{
			//printf("Here2\n");
			execute_cmd(i, cmds, ms);
		}
			
		//Cleaning before the next input
		clean_token_list(&tokens);
		clean_block_list(&blocks);
		//print_cmds(cmds);
		//clean_cmd_list(&cmds);
	}

	// Freeing struct
	free(ms);
	return (0);
}
