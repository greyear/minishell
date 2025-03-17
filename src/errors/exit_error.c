#include "../../include/minishell.h"

/**
 * @brief Prints an error message when a non-numeric argument is provided to the `exit` command.
 * 
 * This function is used to print an error message when the `exit` command is called with an argument that
 * is not a valid numeric value. It informs the user that a numeric argument is required to exit the shell.
 * The function uses the argument passed to the `exit` command to display it in the error message.
 * 
 * @param array The array of arguments passed to the `exit` command. The function specifically uses `array[1]`
 *        (the first argument after the command itself) to display the invalid argument in the error message.
 * 
 * @return This function does not return a value. It directly prints an error message to `stderr`.
 */

void	print_numeric_error(char **array)
{
	ft_putstr_fd(OWN_ERR_MSG, STDERR_FILENO);
	ft_putstr_fd("exit: ", STDERR_FILENO);
	ft_putstr_fd(array[1], STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
}
 
 /**
  * @brief Prints an error message when the `exit` command is called with too many arguments.
  * 
  * This function is used to print an error message when the `exit` command is invoked with more than
  * the expected number of arguments. It informs the user that too many arguments were provided and
  * that the `exit` command expects only a numeric argument or no argument at all.
  * 
  * @return This function does not return a value. It directly prints an error message to `stderr`.
  */
 
void	print_too_many_args_error(void)
{
	ft_putstr_fd(OWN_ERR_MSG, STDERR_FILENO);
	ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
}