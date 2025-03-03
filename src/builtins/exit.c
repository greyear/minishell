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

static void	exit_shell(long long exit_nbr, int error, char **array, t_ms *ms)
{
	if (error == 1 && ms)
	{
		ft_putstr_fd(OWN_ERR_MSG, 2);
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(array[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		clean_cmd_list(&(ms->cmds));
		clean_struct(ms);
		exit(2);
	}
	if (exit_nbr > 255 || exit_nbr < 0)
	{
		clean_cmd_list(&(ms->cmds));
		clean_struct(ms);
		exit(exit_nbr % 256);
	}
	clean_cmd_list(&(ms->cmds));
	clean_struct(ms);
	exit(exit_nbr);
}

void	check_exit(char **array, t_ms *ms)
{
	long long	exit_nbr;
	int			error;
	int			exit_code;
	int			i;

	i = 0;
	if (!array || !*array)
		return;
	if (ft_strcmp(array[0], "exit") != 0) //we already checked it in handle builtin?
		return;
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	error = 0;
	if (array[1])
	{
		while (array[1][i])
		{
			if (!ft_isdigit(array[1][i]))
				exit_shell(2, 1, array, ms);
			i++;
		}
		if (array[2])
		{
			ft_putstr_fd(OWN_ERR_MSG, 2);
			ft_putstr_fd("exit: too many arguments\n", 2);
			ms->exit_status = 1;
			return;
		}
		exit_nbr = ft_strtoll(array[1], &error);	// If non-numeric or out of range, print error and exit(2)
		exit_shell(exit_nbr, error, array, ms);
	}
	exit_code = ms->exit_status;
	//free_struct(ms); Changed it as it gave a leak, i can explain!
	clean_cmd_list(&(ms->cmds));
	clean_struct(ms);
	exit(exit_code);
}


//check more the case when I call 1- echo, 2-exit 42 - leaks in exit
