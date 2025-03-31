/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 14:07:23 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 14:07:25 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

volatile sig_atomic_t	g_sgnl;

void	print_tokens(t_token *token_list)
{
	t_token	*cur = token_list;

	printf("Tokens:\n");
	while (cur)
	{
		printf("Type: %d, Data: %s, Quotes: %c, Redir: %d, Ambig: %d, File: %s\n", cur->type, cur->data, cur->quote, cur->specific_redir, cur->ambiguous, cur->file);
		cur = cur->next;
	}
}

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
			make_multiple_children(i, ms->cmds, ms);
	}
	close_every_cmds_fds(ms->cmds);
}

/**
 * @brief Processes and tokenizes the user input for execution.
 * 
 * This function applies multiple processing steps to the input, including 
 * initial validation, tokenization, and the creation of command structures. 
 * It also handles interruption signals and cleans up partially allocated 
 * data if necessary.
 * 
 * @param input A pointer to the user's input string.
 * @param ms A pointer to the main shell structure containing shell state.
 * 
 * @return 1 if processing is successful, 0 if an error occurs or an 
 *         interruption is detected.
 */
static int	tokenize_and_process_input(char **input, t_ms *ms)
{
	if (!process_input(input, ms))
		return (0);
	if (!tokenize_input(input, ms))
		return (0);
	if (!create_blocks_and_cmds_lists(ms))
		return (0);
	if (g_sgnl == SIGINT)
	{
		clean_struct_partially(ms);
		g_sgnl = 0;
		return (0);
	}
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
 * failure or system error) or the user exits (Ctrl+D).
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
		// FOR USUAL EXECUTION
		/*signal_mode(INTERACTIVE);
		input = readline("minishell> ");
		signal_mode(IGNORE);*/
		//FOR TESTER
		if (isatty(fileno(stdin)))
			input = readline("minishell> ");
		else
		{
			char	*line = get_next_line(fileno(stdin));
			if (!line)
				break ;
			input = ft_strtrim(line, "\n");
			free(line);
		}
		if (!input)
		{
			ft_putstr_fd("exit\n", STDOUT_FILENO);
			break ;
		}
		if (!tokenize_and_process_input(&input, ms))
			continue ;
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
