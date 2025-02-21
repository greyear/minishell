#include "../../include/minishell.h"

void    handle_env(char	**args, t_ms *ms)
{
    ms->exit_status = 0;
    if (!args || !*args)
		return;
	if (ft_strcmp(args[0], "env") != 0)
		return;
	//check if args[1], what error msg then?
	print_array(ms->envp);
}
