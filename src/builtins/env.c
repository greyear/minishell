#include "../../include/minishell.h"

void    handle_env(t_ms *ms)
{
    ms->exit_status = 0;
    print_array(ms->envp);
}
