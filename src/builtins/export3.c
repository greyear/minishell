
#include "../../include/minishell.h"


void	print_error3(t_ms *ms, char *arg)
{
	ms->exit_status = 1;
	ft_putstr_fd("bash: export: '", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

static void print_var(char *var)
{
    int x;

    x = 0;
	while (var[x])
    {
        if (var[x] != '=')
        {
            printf("%c", var[x]);
        }
        else
        {
            printf("%c\"", var[x]);
            x++;
            while (var[x])
            {
                printf("%c", var[x]);
                x++;
            }
            printf("\"");
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
        printf("declare -x ");
        print_var(ms->exported[i]);  // Printing the variable
        printf("\n");
        i++;
    }
}
