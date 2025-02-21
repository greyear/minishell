/*
HANDLES THESE CASES:
env                 = prints out envp stored in ms->envp
export              = prints out exported variables stored in ms->exported
export KEY          = adds KEY to exported without value, if it already exists with or without VALUE, do nothing
export KEY=         = changes KEY's value to empty in exported and envp, creates it with this empty value if it don't already exist
export KEY=VALUE    = changes KEY's value to VALUE in exported and envp, creates it with this value if it don't already exist

and export always prints in alphabetical order, with declare -x before KEY and VALUE is in double quotes ""

KEY contains only alphabets or numbers or _ and the first character cant be a number
*/
/*
in env handlind, call handle_envp(t_ms *ms) function with the ms struct
in export handling, call handle_exported(char **args, t_ms *ms) function
with args and the ms struct
args are for example args[0]="export" args[1]="HEY=hi" args[2]=NULL

EDGE CASES EXPLAINED IN DOCS
*/

#include "../../include/minishell.h"

static char	**copy_to_temp(char *arg, char ***env, char *name, int *flag)
{
	char	**temp;
	int		i;
	int		len;

	i = 0;
	len = get_key_length(arg);
	temp = allocate_temp_env(*env, 2);
	if (!temp)
		return (NULL);
	while ((*env)[i])
	{
		if (check_env((*env)[i], name, len, *flag)) //check if we replace the value
		{
			*flag = 2;
			temp[i] = ft_strdup(arg);
		}
		else
			temp[i] = ft_strdup((*env)[i]);
		if (!temp[i])
		{
			ft_free_map(temp);
			return (NULL);
		}
		i++;
	}
	temp[i] = NULL;
	return (temp);
}

static void	change_values(char *arg, char ***env, char *name, int flag)
{
	char	**temp;
	int		i;

	i = 0;
	temp = copy_to_temp(arg, env, name, &flag);
	if (!temp || !*temp)
		return;
	if (flag != 2)
	{
		while (temp[i])
			i++;
		temp[i] = ft_strdup(arg);
		if (!temp[i])
		{
			ft_free_map(temp);
			return;
		}
		i++;
		temp[i] = NULL;
	}
	ft_free_map(*env);
	*env = temp;
}

static void    change_values_env_ex(char *arg, t_ms *ms)
{
    int     len;
    char    *name;

	len = get_key_length(arg);
	if (len == 0)
		return;
	name = extract_key(arg, len);
	if (!name)
		return;
	if (check_if_valid_key(name) == 1)
	{
		free(name);
		print_error3(ms, arg);
		return;
	}
    change_values(arg, &ms->exported, name, 1);
    change_values(arg, &ms->envp, name, 0);
	free(name);
}


static void	handle_export2(char **args, t_ms *ms)
{
	int		i;

	i = 1;
	while (args[i])
	{
		if (ft_strchr(args[i], '=')) //add to env and exported
			change_values_env_ex(args[i], ms);
		else //add only to exported if there is no = mark
			//add_to_exported(args[i], ms);
        sort_exported_alphaorder(ms);
		i++;
	}
}

void    handle_export(char **args, t_ms *ms) /// args are for exapmle args[0]="export" args[1]="HEY=hi" args[2]=NULL
{
    int     arg_count;

    ms->exit_status = 0;
    arg_count = 0;
	if (!args)
		return;
	if (args[0] && ft_strcmp(args[0], "export") != 0)
		return;
    while (args[arg_count])
        arg_count++;
    if (arg_count == 1)
    {
        sort_exported_alphaorder(ms);
        print_exported(ms);
		return;
    }
	handle_export2(args, ms);
}
