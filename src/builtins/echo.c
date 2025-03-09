#include "../../include/minishell.h"

/**
 * @brief Checks if a given argument is a valid `-n` flag for the `echo` command.
 * 
 * A valid `-n` flag starts with `-n` and contains only `n` characters after the `-`.
 * If the argument does not match this pattern, it is considered invalid.
 * 
 * @param arg The argument string to check.
 * @return int Returns 1 if the argument is a valid `-n` flag, otherwise 0.
 */

static int	is_valid_n_flag(char *arg)
{
	int		i;

	if (arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 1;
	while (arg[i] && arg[i] == 'n')
		i++;
	if (arg[i] == '\0')
		return (1);
	return (0);
}

/**
 * @brief Handles the `-n` flag for the `echo` command.
 * 
 * This function checks for consecutive `-n` flags in the command arguments.
 * If valid `-n` flags are found, it increments the index to skip them and 
 * returns 1 to indicate that the newline should be suppressed.
 * 
 * @param args A NULL-terminated array of command arguments.
 * @param i A pointer to the argument index, which is incremented past `-n` flags.
 * @return int Returns 1 if at least one valid `-n` flag is found, otherwise 0.
 */

static int	handle_n_flags(char **args, int *i)
{
	int		check;

	check = 0;
	while (args[*i] && is_valid_n_flag(args[*i]))
	{
		check = 1;
		(*i)++;
	}
	return (check);
}

/**
 * @brief Handles the execution of the `echo` command.
 * 
 * This function prints the given arguments to standard output, handling the `-n` flag 
 * (which suppresses the trailing newline). If no arguments are provided, it prints a 
 * newline by default.
 * 
 * @param args A NULL-terminated array of command arguments, with args[0] expected to be "echo".
 * @param ms A pointer to the shell structure, used to update the exit status.
 */

void	handle_echo(char **args, t_ms *ms)
{
	int		i;
	int		check;

	if (!args || !*args || ft_strcmp(args[0], "echo") != 0)
		return;
	ms->exit_status = 0;
	i = 1;
	check = handle_n_flags(args, &i);
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (check == 0)
		ft_putstr_fd("\n", STDOUT_FILENO);
}