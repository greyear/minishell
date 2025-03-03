#include "../../include/minishell.h"

/*static char	**filter_env_entries(char **env, char *name, int len, int flag)
{
	char	**temp;
	int		i;
	int		x;

	temp = allocate_temp_env(env, 1);
	if (!temp)
		return (NULL);
	i = 0;
	x = 0;
	while (env[i])
	{
		if (check_env(env[i], name, len, flag))
		{
			i++;
			continue;
		}
		temp[x] = ft_strdup(env[i]);
		if (!temp[x])
		{
			clean_arr(&temp);
			return (NULL);
		}
		x++;
		i++;
	}
	temp[x] = NULL;
	return (temp);
}*/

static int	should_filter_entry(char *entry, char *name, int len, int flag)
{
	if (check_env(entry, name, len, flag))
		return (1);
	return (0);
}

static int	copy_env_entries(char **dest, char **src, char *name, int len, int flag)
{
	int i;
	int x;

	i = 0;
	x = 0;
	while (src[i])
	{
		if (should_filter_entry(src[i], name, len, flag))
		{
			i++;
			continue;
		}
		dest[x] = ft_strdup(src[i]);
		if (!dest[x])
		{
			clean_arr(&dest);
			return (0);
		}
		x++;
		i++;
	}
	dest[x] = NULL;
	return (1);
}

static char	**filter_env_entries(char **env, char *name, int len, int flag)
{
	char	**filtered_env;

	filtered_env = allocate_temp_env(env, 1);
	if (!filtered_env)
		return (NULL);
	if (!copy_env_entries(filtered_env, env, name, len, flag))
		return (NULL);
	return (filtered_env);
}


void	rm_from_env_ex(char ***env, char *name, int len, int flag)
{
	char	**new_env;

	if (!env || !(*env))
		return;
	new_env = filter_env_entries(*env, name, len, flag);
	if (!new_env)
		return;
	clean_arr(env);
	*env = new_env;
}
