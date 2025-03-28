/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 12:55:53 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 12:55:55 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Cleans up resources and exits the program.
 * 
 * This function is responsible for cleaning up all allocated resources and 
 * performing necessary shutdown operations before exiting the program. It frees 
 * memory used by the command list, clears the history, and cleans up
 * the shell's internal structure. After all cleanup tasks are completed,
 * it exits the program with the specified exit status.
 * 
 * @param exit_status The exit status code to return to the operating system 
 *                    upon program termination.
 * @param ms A pointer to the `t_ms` structure that holds shell data, including 
 *           the status and resources to clean up.
 */

static void	clean_up_and_exit(int exit_status, t_ms *ms)
{
	clean_cmd_list(&(ms->cmds));
	history_exit(ms);
	clean_struct(ms);
	exit(exit_status);
}

/**
 * @brief Handles the exit code logic for the `exit` command.
 * 
 * This function processes the exit code provided by the user. It handles errors 
 * such as non-numeric input or invalid exit values. If the exit code is out of 
 * range (greater than 255 or less than 0), it adjusts the exit code to fit
 * within the valid range by taking the modulo of 256. After processing 
 * the exit code, the function will clean up resources and exit the program 
 * with the determined exit status.
 * 
 * @param exit_nbr The exit number provided by the user (or calculated), which 
 *                 can be modified to fit valid exit range.
 * @param error An error flag indicating if the argument was invalid 
 *                (1 for error).
 * @param array The arguments passed to the `exit` command, used to print error 
 *              messages.
 * @param ms A pointer to the `t_ms` structure that stores shell data, including 
 *           the exit status.
 */
static void	handle_exit_code(long long exit_nbr, int err, char **arr, t_ms *ms)
{
	if (err == 1 && ms)
	{
		print_numeric_error(arr);
		clean_up_and_exit(2, ms);
	}
	if (exit_nbr > 255 || exit_nbr < 0)
		clean_up_and_exit(exit_nbr % 256, ms);
	clean_up_and_exit(exit_nbr, ms);
}

/**
 * @brief Checks if the argument passed to the `exit` command is a valid numeric 
 *        exit code.
 * 
 * This function verifies that the argument provided to the `exit` command 
 * consists only of digits, optionally preceded by a '+' or '-' sign. If any 
 * non-digit character is found, it calls the `handle_exit_code` function with 
 * an error code, which then handles the invalid argument case.
 * 
 * @param arg  The string argument to be checked for numeric validity. This is 
 *             the argument passed after "exit".
 * @param arr The arguments passed to the `exit` command, used for error 
 *              handling.
 * @param ms    A pointer to the `t_ms` structure, which holds the shell's exit 
 *              status and other data.
 */
static void	check_numeric_argument(char *arg, char **arr, t_ms *ms)
{
	int		i;

	i = 0;
	if (arg[i] && (arg[i] == '+' || arg[i] == '-'))
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			handle_exit_code(2, 1, arr, ms);
		i++;
	}
}

/**
 * @brief Handles the `exit` command logic in the shell.
 * 
 * This function processes the exit command:
 * - If an argument is provided, it checks if it's a valid numeric exit 
 *   code.
 * - If there are multiple arguments, it prints an error message and sets 
 *   the exit status to 1.
 * - It converts the valid argument to a long long integer and processes 
 *   the exit code.
 * - If no arguments are provided, the shell exits with the current exit 
 *   status.
 * 
 * @param array The arguments passed to the `exit` command, where the 
 *              first is "exit" and the second (optional) is the exit code.
 * @param ms    A pointer to the `t_ms` structure that contains shell data, 
 *              including the exit status.
 */
void	check_exit(char **array, t_ms *ms)
{
	long long	exit_nbr;
	int			err;

	err = 0;
	if (isatty(STDIN_FILENO))
		ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (array[1])
	{
		check_numeric_argument(array[1], array, ms);
		if (array[2])
		{
			print_too_many_args_error();
			ms->exit_status = 1;
			return ;
		}
		exit_nbr = convert_to_ll(array[1], &err);
		handle_exit_code(exit_nbr, err, array, ms);
	}
	clean_up_and_exit(ms->exit_status, ms);
}
