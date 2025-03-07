#include "../../include/minishell.h"

static char	*increase_shlvl(char *shlvl_value)
{
    int		level;
	
	level = ft_atoi(shlvl_value);
	return (ft_itoa(level + 1));
}

static int	update_shlvl(char ***env)
{
    int		i;
	char	*new_value;
	
	i = 0;
    while ((*env)[i])
    {
        if (ft_strncmp((*env)[i], "SHLVL=", 6) == 0)
        {
           	new_value = increase_shlvl((*env)[i] + 6);
            if (new_value)
            {
                free((*env)[i]);
                (*env)[i] = ft_strjoin("SHLVL=", new_value);
                free(new_value);
            }
            return(1);
        }
        i++;
    }
	return (0);
}

static void	make_args(char ***export, t_ms *ms)
{
	*export = malloc(sizeof(char *) * 3);
	if (!*export)
	{
		ms->exit_status = 1;
		return;
	}
	(*export)[0] = ft_strdup("export");
	if (!(*export)[0])
	{
		clean_arr(&(*export));
		ms->exit_status = 1;
		return;
	}
	(*export)[1] = ft_strdup("SHLVL=1");
	(*export)[2] = NULL;
	if (!(*export)[1])
	{
		clean_arr(&(*export));
		ms->exit_status = 1;
		return;
	}
	(*export)[2] = NULL;
}

void	check_shlvl(t_ms *ms)
{
	char	**export;

	make_args(&export, ms);
	if (!export)
		return;
	update_shlvl(&ms->envp);
	if (!update_shlvl(&ms->exported))
		handle_export(export, ms);
	clean_arr(&export);
}
