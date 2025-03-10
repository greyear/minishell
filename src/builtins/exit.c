#include "../../include/minishell.h"

/**
 * @brief Cleans up resources and exits the program.
 * 
 * This function is responsible for cleaning up all allocated resources and performing necessary shutdown
 * operations before exiting the program. It frees memory used by the command list, clears the history, 
 * and cleans up the shell's internal structure. After all cleanup tasks are completed, it exits the program
 * with the specified exit status.
 * 
 * @param exit_status The exit status code to return to the operating system upon program termination.
 * @param ms A pointer to the `t_ms` structure that holds shell data, including the status and resources to clean up.
 */

static void clean_up_and_exit(int exit_status, t_ms *ms)
{
    clean_cmd_list(&(ms->cmds));
    history_exit(ms);
    /*close(ms->saved_stdin);
	close(ms->saved_stdout);*/
    clean_struct(ms);
    exit(exit_status);
}

/**
 * @brief Handles the exit code logic for the `exit` command.
 * 
 * This function processes the exit code provided by the user. It handles errors such as non-numeric input
 * or invalid exit values. If the exit code is out of range (greater than 255 or less than 0), it adjusts the 
 * exit code to fit within the valid range by taking the modulo of 256. After processing the exit code, the function
 * will clean up resources and exit the program with the determined exit status.
 * 
 * @param exit_nbr The exit number provided by the user (or calculated), which can be modified to fit valid exit range.
 * @param error An error flag indicating if the argument was invalid (1 for error).
 * @param array The arguments passed to the `exit` command, used to print error messages.
 * @param ms A pointer to the `t_ms` structure that stores shell data, including the exit status.
 */

static void handle_exit_code(long long exit_nbr, int error, char **array, t_ms *ms)
{
    if (error == 1 && ms)
    {
        print_numeric_error(array);
        clean_up_and_exit(2, ms);
    }
    if (exit_nbr > 255 || exit_nbr < 0)
        clean_up_and_exit(exit_nbr % 256, ms);
    clean_up_and_exit(exit_nbr, ms);
}

/**
 * @brief Checks if the argument passed to the `exit` command is a valid numeric exit code.
 * 
 * This function verifies that the argument provided to the `exit` command consists only of digits,
 * optionally preceded by a '+' or '-' sign. If any non-digit character is found, it calls the `handle_exit_code`
 * function with an error code, which then handles the invalid argument case.
 * 
 * @param arg  The string argument to be checked for numeric validity. This is the argument passed after "exit".
 * @param array The arguments passed to the `exit` command, used for error handling.
 * @param ms    A pointer to the `t_ms` structure, which holds the shell's exit status and other data.
 */

static void check_numeric_argument(char *arg, char **array, t_ms *ms)
{
    int		i;
	
	i = 0;
    if (arg[i] && (arg[i] == '+' || arg[i] == '-'))
        i++;
    while (arg[i])
    {
        if (!ft_isdigit(arg[i]))
            handle_exit_code(2, 1, array, ms);
        i++;
    }
}

/**
 * @brief Handles the arguments for the `exit` command in the shell.
 * 
 * This function processes the argument passed to the `exit` command:
 * - If there is one argument, it checks if it is a valid numeric exit code.
 * - If there are multiple arguments, it prints an error message indicating that too many arguments were provided.
 * - The exit code is then validated and processed, and the shell will exit with the corresponding status.
 * 
 * If there is no argument, the default behavior is to exit with the current `ms->exit_status`.
 * If an invalid numeric argument is provided, an error is handled and the function ensures a clean exit.
 * 
 * @param array The arguments passed to the `exit` command. The first argument is "exit", and the second (optional) argument is the exit code.
 * @param ms    A pointer to the `t_ms` structure, which holds the shell's exit status and other data.
 */

static void handle_exit_argument(char **array, t_ms *ms)
{
    long long   exit_nbr;
    int	        error;

	error = 0;
    if (array[1])
    {
		check_numeric_argument(array[1], array, ms);
        if (array[2])
        {
            print_too_many_args_error();
            ms->exit_status = 1;
            return;
        }
        exit_nbr = convert_to_ll(array[1], &error);
        handle_exit_code(exit_nbr, error, array, ms);
    }
    clean_up_and_exit(ms->exit_status, ms);
}

/**
 * @brief Handles the `exit` command, which terminates the shell.
 * 
 * If the `exit` command is entered (without additional arguments), the shell prints "exit" to the standard output
 * (if the shell is interactive, i.e., connected to a terminal). Then, it processes any exit argument by calling
 * `handle_exit_argument`.
 * If the command is not `exit` or if the argument is invalid, the function simply returns without performing any action.
 * 
 * @param array The arguments passed to the `exit` command. The first argument should be "exit".
 * @param ms    A pointer to the `t_ms` structure, which holds the exit status and other shell information.
 */

void check_exit(char **array, t_ms *ms)
{
    if (!array || !*array)
        return;
    if (ft_strcmp(array[0], "exit") != 0)
        return;
    if (isatty(STDIN_FILENO))
        ft_putstr_fd("exit\n", STDOUT_FILENO);
    handle_exit_argument(array, ms);
}