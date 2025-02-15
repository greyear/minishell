
#include "../../include/minishell.h"

void    handle_env(t_ms *ms)
{
    ms->exit_status = 0;
    print_array(ms->envp);
}

void	print_error3(t_ms *ms, char *arg)
{
	ms->exit_status = 1;
	ft_putstr_fd("bash: export: '", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

void    print_exported(t_ms *ms)
{
    int     i;
    int     x;

    i = 0;
    x = 0;
    while (ms->exported[i])
    {
        x = 0;
        printf("declare -x ");
        while (ms->exported[i][x])
        {
            if (ms->exported[i][x] != '=')
            {
                printf("%c", ms->exported[i][x]);
                x++;
            }
            else
            {
                printf("%c", ms->exported[i][x]);
                x++;
                printf("\"");
                while (ms->exported[i][x])
                {
                    printf("%c", ms->exported[i][x]);
                    x++;
                }
                printf("\"");
                break;
            }
        }
        printf("\n");
        i++;
    }
}

void    sort_exported_alphaorder(t_ms *ms)
{
    int     i;
    int     j;
    char    *temp;

    i = 0;
    j = 0;
    while (ms->exported[i])
    {
        j = i + 1;
        while (ms->exported[j])
        {
            if (ft_strcmp(ms->exported[i], ms->exported[j]) > 0)
            {
                temp = ms->exported[i];
                ms->exported[i] = ms->exported[j];
                ms->exported[j] = temp;
            }
            j++;
        }
        i++;
    }
}
