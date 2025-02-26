//JUST A SIMPLE VERSION CREATED
//EXPLANATON IN DOCS

#include "../../include/minishell.h"

/*void	handle_echo(char **args, t_ms *ms)
{
	int		i;
	int		check;
	int		has_printed;

	i = 1;
	check = 0;
	has_printed = 0;
	if (!args || !*args)
		return;
	if (ft_strcmp(args[0], "echo") != 0)
		return;
	ms->exit_status = 0;
	while (args[i])
	{
		if (has_printed == 0 && (ft_strcmp(args[i], "-n") == 0))
		{
			if (check == 0)
				check = 1;
			i++;
			continue;
		}
		has_printed = 1;
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
		{
			ft_putstr_fd(" ", STDOUT_FILENO);
		}
		i++;
	}
	if (check == 0)
		ft_putstr_fd("\n", STDOUT_FILENO);
}*/
static int	is_valid_n_flag(char *arg)
{
	int	i;

	if (arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 1;
	while (arg[i] == 'n')
		i++;
	return (arg[i] == '\0'); // Only valid if it contains only 'n's after '-'
}

static int	handle_n_flags(char **args, int *i)
{
	int	check;

	check = 0;
	while (args[*i] && is_valid_n_flag(args[*i]))
	{
		check = 1;
		(*i)++;
	}
	return (check);
}

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
