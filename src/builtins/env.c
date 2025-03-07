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
		print_env_error(args);
		ms->exit_status = 127;
		return;
	}
	print_array(ms->envp);
}