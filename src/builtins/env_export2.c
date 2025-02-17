
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
