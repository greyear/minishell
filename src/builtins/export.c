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
	if (flag != 2)
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
		if (ft_strchr(args[i], '='))
			change_values_env_ex(args[i], ms);
		else
			add_to_exported(args[i], ms);
        sort_exported_alphaorder(ms);
		i++;
	}
}

void    handle_export(char **args, t_ms *ms)
{
    int     arg_count;

    ms->exit_status = 0;
    arg_count = 0;
	if (!ms)
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