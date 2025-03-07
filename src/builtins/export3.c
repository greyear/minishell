
#include "../../include/minishell.h"

static void print_var(char *var)
{
    int x;

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
            break;
        }
        x++;
    }
}

void print_exported(t_ms *ms)
{
    int i;

	i = 0;
    while (ms->exported[i])
    {
        ft_putstr_fd("declare -x ", STDOUT_FILENO);
        print_var(ms->exported[i]);  // Printing the variable
        ft_putchar_fd('\n', STDOUT_FILENO);
        i++;
    }
}