/*
 can change later but now just takes an array like array0] = "exit", array[1] = 1, array[2] = NULL

Handles the exit command, including support for:
No argument (defaults to exit 127). (exit)
Valid numeric argument (exits with that code). (exit 1)
if exit is bigger than 255, exits with %module 256
for example exit 300 would exit with 44

Too many arguments (prints an error). (exit 1 1)
Non-numeric argument (prints an error). (exit 1x)

edge cases / LLONG_MAX etc explained in google docs !!!
*/

/*
 SHOULD ADD THIS ,SYNTAX ERROR THINGS
 if
 exit (1)
 The error bash: syntax error near unexpected token '1' happens because of the wrong syntax.
 */

#include "../../include/minishell.h"

void	free_struct(t_ms *ms)
{
	ft_free_array(ms->envp);
	ft_free_array(ms->exported);
}

static void	exit_shell(long long exit_nbr, int error, char **array)
{
	if (error == 1)
	{
		ft_putstr_fd("bash: exit: ", 2);
		ft_putstr_fd(array[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(2);
	}
	if (exit_nbr > 255 || exit_nbr < 0)
		exit(exit_nbr % 256);
	exit(exit_nbr);
}

void	check_exit(char **array, t_ms *ms)
{
	long long	exit_nbr;
	int		error;

	if (!array || !*array)
		return;
	if (ft_strcmp(array[0], "exit") != 0)
		return;
	ft_putstr_fd("exit\n", 1);
	if (array[1])
	{
		if (array[2])
		{
			ft_putstr_fd("bash: exit: too many arguments\n", 2);
			ms->exit_status = 1;
			return;
		}
		free_struct(ms);
		exit_nbr = ft_strtoll(array[1], &error);	// If non-numeric or out of range, print error and exit(2)
		exit_shell(exit_nbr, error, array);
	}
	free_struct(ms);
	exit(ms->exit_status);
}
