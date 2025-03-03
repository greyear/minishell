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

static char	**copy_to_temp(char *arg, char ***env, char *key, int *flag)
{
	char	**temp;
	int		i;

	i = 0;
	temp = allocate_temp_env(*env, 2);
	if (!temp)
		return (NULL);

	while ((*env)[i])
	{
		temp[i] = duplicate_or_replace((*env)[i], arg, key, flag);
		if (!temp[i])
		{
			clean_arr(&temp);
			return (NULL);
		}
		i++;
	}
	temp[i] = NULL;
	return (temp);
}

static void	change_values(char *arg, char ***env, char *key, int flag)
{
	char	**temp;
	int		i;

	i = 0;
	temp = copy_to_temp(arg, env, key, &flag);
	if (!temp || !*temp)
		return;
	if (flag != 2) // add value to end
	{
		while (temp[i])
			i++;
		temp[i] = ft_strdup(arg);
		if (!temp[i])
		{
			clean_arr(&temp);
			return;
		}
		i++;
		temp[i] = NULL;
	}
	clean_arr(env);
	*env = temp;
}

static void    change_values_env_ex(char *arg, t_ms *ms)
{
    int     len;
    char    *key;

	len = get_key_length(arg);
	key = extract_key(arg, len);
	if (check_if_valid_key(key) == 1)
	{
		free(key);
		print_export_error(ms, arg);
		return;
	}
    change_values(arg, &ms->exported, key, 1);
    change_values(arg, &ms->envp, key, 0);
	free(key);
}

static void	process_arguments(char **args, t_ms *ms)
{
	int		i;

	i = 1;
	while (args[i])
	{
		if (ft_strchr(args[i], '=')) //add to env and exported
			change_values_env_ex(args[i], ms);
		else //add only to exported if there is no = mark
			add_to_exported(args[i], ms);
        sort_exported_alphaorder(ms);
		i++;
	}
}

void    handle_export(char **args, t_ms *ms) /// args are for exapmle args[0]="export" args[1]="HEY=hi" args[2]=NULL
{
    int     arg_count;

    ms->exit_status = 0;
    arg_count = 0;
	if (!ms)  // Running in a child, don't modify ms->envp
		return;
    if (!args || !*args)
		return;
	if (ft_strcmp(args[0], "export") != 0)
		return;
    while (args[arg_count])
        arg_count++;
    if (arg_count == 1)
    {
        sort_exported_alphaorder(ms);
        print_exported(ms);
		return;
    }
	process_arguments(args, ms);
}
