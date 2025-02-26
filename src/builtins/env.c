#include "../../include/minishell.h"

void    handle_env(char	**args, t_ms *ms)
{
    ms->exit_status = 0;
    if (!args || !*args)
		return;
	if (ft_strcmp(args[0], "env") != 0)
		return;
	if (args[1])
	{
		ft_putstr_fd("env: '", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd("': No such file or directory\n", 2);
		ms->exit_status = 127;
		return;
	}
	//check if args[1], what error msg then?
	print_array(ms->envp);
}
