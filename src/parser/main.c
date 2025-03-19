#include "../../include/minishell.h"

//#include <asm-generic/termbits.h> //delete for school computers

volatile sig_atomic_t	g_sgnl;

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

/**
 * @brief Executes a list of commands.
 * 
 * This function processes a list of commands, determining whether to execute 
 * them as built-in functions or external commands. It handles the execution 
 * of a single command or multiple commands by creating child processes as needed.
 * 
 * If there is only one command and it is a built-in, it will be executed directly. 
 * If there are multiple commands, child processes will be created for each command 
 * to execute them in parallel. After executing the commands, it ensures file descriptors 
 * are closed and handles the appropriate exit statuses.
 * 
 * @param ms A pointer to the shell's main structure containing the command list 
 *           and other relevant state information.
 */

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

/**
 * @brief Counts the number of heredoc tokens in the token list.
 * 
 * This function iterates through the token list and counts how many tokens 
 * are of type `HEREDOC`. The function returns the total count of heredoc 
 * tokens in the list.
 * 
 * @param token A pointer to the head of the token list to be scanned.
 * 
 * @return The number of heredoc tokens found in the list.
 */

static int	count_heredocs(t_token *token)
{
	t_token	*cur;
	int	heredoc_count;

	cur = token;
	heredoc_count = 0;
	while (cur)
	{
		if (cur->type == HEREDOC)
			heredoc_count++;
		cur = cur->next;
	}
	return (heredoc_count);
}

/**
 * @brief Allocates memory for heredoc file paths and handles errors.
 * 
 * This function calculates the number of heredocs from the given token list 
 * and allocates memory for storing the paths to the heredoc files in the 
 * `ms->heredoc_files` array. If the number of heredocs exceeds the maximum 
 * allowed (16), an error message is printed, and the program exits. If memory 
 * allocation fails, a malloc error is printed, and the program exits. 
 * Otherwise, the function initializes the `ms->heredoc_files` array with NULL 
 * values.
 * 
 * @param ms A pointer to the main shell structure, which will store the heredoc file paths.
 * @param token A pointer to the token list, which is used to count the heredocs.
 */

static void	malloc_heredocs(t_ms *ms, t_token *token)
{
	int		heredoc_count;

	heredoc_count = count_heredocs(token);
	if (heredoc_count > 16)
	{
		ft_putstr_fd(OWN_ERR_MSG, STDERR_FILENO);
		ft_putstr_fd(HEREDOC_ERR, STDERR_FILENO);
		clean_struct(ms);
		exit(2);
	}
	ms->heredoc_files = malloc(sizeof(char *) * (heredoc_count + 1)); // Support 100 heredocs max
	if (!ms->heredoc_files)
	{
		print_malloc_error();
		clean_struct(ms);
		exit(1);
	}
	ft_memset(ms->heredoc_files, 0, sizeof(char *) * (heredoc_count + 1)); // Set all entries to NULL
	ms->heredoc_files[0] = NULL;
}

/**
 * @brief Tokenizes the input string and processes redirections and heredocs.
 * 
 * This function tokenizes the given input string and stores the tokens in the 
 * `ms->tokens` structure. If tokenization fails, it prints an error message and 
 * returns 0. It also frees the original input string after tokenization. After 
 * tokenization, it processes heredocs and sets up files for redirections.
 * 
 * @param input A pointer to the input string to be tokenized.
 * @param ms A pointer to the main shell structure, which holds the tokens.
 * @return 1 if tokenization is successful, 0 if there is an error.
 */

static int	tokenize_input(char **input, t_ms *ms)
{
	ms->tokens = tokenization(*input, ms);
	free(*input);
	if (!ms->tokens)
	{
		ft_putstr_fd(TOKENS_ERR, STDERR_FILENO);
		return (0);
	}
	malloc_heredocs(ms, ms->tokens);
	put_files_for_redirections(ms->tokens);
	return (1);
}

/**
 * @brief Creates the blocks and commands lists from the tokenized input.
 * 
 * This function creates the list of blocks and commands from the tokenized input. 
 * If an error occurs during the creation of either list, it will clean up 
 * any previously allocated resources (tokens and blocks) and return 0. 
 * Otherwise, it returns 1 to indicate successful creation of both lists.
 * 
 * @param ms A pointer to the main shell structure, used to store the blocks and commands lists.
 * @return 1 if both blocks and commands lists are successfully created, 0 if there was an error.
 */

static int	create_blocks_and_cmds_lists(t_ms *ms)
{
	int		err_syntax;

	err_syntax = 0;
	ms->blocks = create_blocks_list(ms->tokens, NULL, &err_syntax);
	if (err_syntax)
	{
		ft_putstr_fd(BLOCKS_ERR, STDERR_FILENO);
		clean_token_list(&(ms->tokens));
		return (0);
	}
	ms->cmds = create_cmd_list(ms->blocks, ms);
	if (!ms->cmds)
	{
		ft_putstr_fd(CMDS_ERR, STDERR_FILENO);
		clean_token_list(&(ms->tokens));
		clean_block_list(&(ms->blocks));
		return (0);
	}
	return (1);
}

/**
 * @brief Processes user input for syntax validation and history addition.
 * 
 * This function checks if the input is empty or contains syntax errors. 
 * If the input is empty (i.e., only Enter was pressed), it is ignored.
 * If a syntax error is found, the error code is set and the input is discarded. 
 * Otherwise, the input is added to the history.
 * 
 * @param input A pointer to the string containing the user input.
 * @param ms A pointer to the main shell structure, used to store the exit status.
 * @return 1 if the input is valid and processed, 0 if there was an error or the input was empty.
 */

static int	process_input(char **input, t_ms *ms)
{
	int		err_syntax;

	err_syntax = 0;
	if (g_sgnl == SIGINT)
	{
		ms->exit_status = 130;
		g_sgnl = 0;
	}
	if ((*input)[0] == '\0') // Ignore empty input (Enter)
	{
		free(*input);
		return (0);
	}
	err_syntax = validate_input(*input);
	if (err_syntax)
	{
		free(*input);
		ms->exit_status = 2;
		return (0);
	}
	add_line_to_history(*input, ms);
	return (1);
}

static void	run_minishell(t_ms *ms)
{
	char	*input;

	while (1)
	{
		if (ms->exit_status == MALLOC_ERR
			|| ms->exit_status == SYSTEM_ERR)
			break;
		// Reading the input
		// FOR USUAL EXECUTION
		/*signal_mode(INTERACTIVE);
		input = readline("minishell> ");
		signal_mode(IGNORE);*/
		//FOR TESTER
		if (isatty(fileno(stdin))) // If running interactively
			input = readline("minishell> ");
		else // If receiving input from another program
		{
			char *line = get_next_line(fileno(stdin));
			if (!line) // Handle EOF (Ctrl+D in non-interactive mode)
				break;
			input = ft_strtrim(line, "\n"); // Remove newline from input
			free(line);
		}
		if (!input) // EOF check (Ctrl+D)
		{
			ft_putstr_fd("exit\n", STDOUT_FILENO);
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
}

int	main(int argc, char **argv, char **envp)
{
	t_ms	*ms;
	int		exit_code;

	if (argc != 1 && argv)
	{
		ft_putstr_fd("Usage: ./minishell\n", STDERR_FILENO);
		return (1);
	}
	if (!init_terminal_signals())
		return (1);
	ms = initialize_struct(envp);
	run_minishell(ms);
	clean_cmd_list(&(ms->cmds));
	history_exit(ms);
	exit_code = ms->exit_status;
	if (exit_code == MALLOC_ERR || exit_code == SYSTEM_ERR)
		exit_code = 1;
	clean_struct(ms);
	rl_clear_history();
	return (exit_code);
}
