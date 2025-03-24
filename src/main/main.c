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

/**
 * @brief Executes a list of commands.
 * 
 * This function processes a list of commands, determining whether to execute 
 * them as built-in functions or external commands. It handles the execution 
 * of a single command or multiple commands by creating child processes as 
 * needed.
 * 
 * If there is only one command and it is a built-in, it will be executed 
 * directly. If there are multiple commands, child processes will be created 
 * for each command to execute them in parallel. After executing the commands, 
 * it ensures file descriptors are closed and handles the appropriate exit 
 * statuses.
 * 
 * @param ms A pointer to the shell's main structure containing the command 
 *           list and other relevant state information.
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
 * @brief Processes user input for syntax validation and history addition.
 * 
 * This function checks if the input is empty or contains syntax errors. 
 * If the input is empty (i.e., only Enter was pressed), it is ignored.
 * If a syntax error is found, the error code is set and the input is discarded. 
 * Otherwise, the input is added to the history.
 * 
 * @param input A pointer to the string containing the user input.
 * @param ms A pointer to the main shell structure, used to store the 
 *           exit status.
 * @return 1 if the input is valid and processed, 0 if there was an error 
 *         or the input was empty.
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
	if ((*input)[0] == '\0')
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

/**
 * @brief Initializes terminal settings for signal handling.
 * 
 * This function configures the terminal to disable control character echoing 
 * (such as `Ctrl+C` and `Ctrl+Z`), which is useful for handling terminal
 * signals in custom shell programs. It uses the `tcgetattr` and `tcsetattr` 
 * functions to modify the terminal settings. If these functions fail, 
 * an error is printed.
 * 
 * @return Returns 1 if the terminal settings were successfully initialized, 
 *         or 0 if an error occurred.
 */
static int	init_terminal_signals(void)
{
	struct termios	term;

	if (isatty(STDIN_FILENO))
	{
		if (tcgetattr(STDIN_FILENO, &term) == -1)
		{
			perror("tcgetattr failed");
			return (0);
		}
		term.c_lflag &= ~ECHOCTL;
		if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
		{
			perror("tcsetattr failed");
			return (0);
		}
	}
	return (1);
}

/**
 * @brief Main loop for executing the minishell commands.
 * 
 * This function runs an interactive or non-interactive loop to read input, 
 * process it, tokenize the input, and execute the commands. It handles EOF 
 * detection (Ctrl+D), cleans up after each command execution, and continues 
 * running until the shell encounters a critical error (e.g., memory allocation 
 * failure or system error) or the user exits (Ctrl+D). It also manages signals 
 * like SIGINT to ensure proper cleanup.
 * 
 * @param ms A pointer to the `t_ms` structure, which holds the shell's state, 
 *           including the exit status and other shell-related data.
 * 
 * @return None. The function modifies the state of the shell based on the 
 *         input and execution results.
 */
static void	run_minishell(t_ms *ms)
{
	char	*input;

	while (1)
	{
		if (ms->exit_status == MALLOC_ERR
			|| ms->exit_status == SYSTEM_ERR)
			break ;
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
				break ;
			input = ft_strtrim(line, "\n"); // Remove newline from input
			free(line);
		}
		if (!input) // EOF check (Ctrl+D)
		{
			ft_putstr_fd("exit\n", STDOUT_FILENO);
			break ;
		}
		if (!process_input(&input, ms))
			continue ;
		if (!tokenize_input(&input, ms))
			continue ;
		if (!create_blocks_and_cmds_lists(ms))
			continue ;
		if (g_sgnl == SIGINT)
		{
			clean_struct_partially(ms);
			g_sgnl = 0;
			continue;
		}
		execute_commands(ms);
		clean_struct_partially(ms);
	}
}

/**
 * @brief Initializes and runs the minishell program.
 *
 * This function checks the argument count and ensures correct usage of the 
 * shell. It initializes terminal signals and the minishell structure, then 
 * runs the shell main loop (`run_minishell`). After execution, it cleans up 
 * resources like command lists, history, and the minishell structure. The 
 * function handles error codes and exits with the appropriate exit status 
 * based on the result of the execution.
 *
 * @param argc The argument count, expected to be 1 for proper usage.
 * @param argv The argument vector, not used in this case but required for 
 * standard `main` format.
 * @param envp The environment variables passed to the program, used for 
 * initialization.
 * @return The exit status of the program, either 1 on error or the exit 
 * status set by the shell.
 */
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
