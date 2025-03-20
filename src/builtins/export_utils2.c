#include "../../include/minishell.h"

/**
 * @brief Prints the variable name and value, enclosing the value in quotes 
 *        if it contains an '='.
 *
 * This function prints a variable, including its name and value. If the 
 * variable contains an equal sign (`=`), indicating a key-value pair, the 
 * value is enclosed in double quotes. The function processes the input string 
 * character by character and handles both the variable name and value 
 * appropriately. If an equal sign is encountered, the value is quoted and 
 * printed after the equal sign. Otherwise, the variable is printed without 
 * quotes.
 *
 * @param var The string representing the variable in the format `key=value`.
 */
static void	print_var(char *var)
{
	int		x;

	x = 0;
	while (var[x])
	{
		if (var[x] != '=')
			ft_putchar_fd(var[x], STDOUT_FILENO);
		else
		{
			ft_putchar_fd(var[x], STDOUT_FILENO);
			ft_putchar_fd('"', STDOUT_FILENO);
			x++;
			while (var[x])
			{
				ft_putchar_fd(var[x], STDOUT_FILENO);
				x++;
			}
			ft_putchar_fd('"', STDOUT_FILENO);
			break ;
		}
		x++;
	}
}

/**
 * @brief Prints all exported environment variables in a specific format.
 *
 * This function iterates over the list of exported environment variables in 
 * the `ms->exported` array and prints each one using the `declare -x` format, 
 * followed by the variable name and value (with the value enclosed in quotes). 
 * For each exported variable, the function calls `print_var` to handle the 
 * printing of the variable name and value.
 *
 * @param ms The shell's main structure containing the list of exported 
 *           environment variables.
 */
void	print_exported(t_ms *ms)
{
	int		i;

	i = 0;
	while (ms->exported[i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		print_var(ms->exported[i]);
		ft_putchar_fd('\n', STDOUT_FILENO);
		i++;
	}
}
