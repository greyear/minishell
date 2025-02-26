//JUST A SIMPLE VERSION CREATED
//EXPLANATON IN DOCS

#include "../../include/minishell.h"

void	handle_echo(char **args, t_ms *ms)
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
		//printf("%s", args[i]);
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
		{
			ft_putstr_fd(" ", STDOUT_FILENO);
		}
			//printf(" ");
		i++;
	}
	if (check == 0)
		ft_putstr_fd("\n", STDOUT_FILENO);
		//printf("\n");
}
