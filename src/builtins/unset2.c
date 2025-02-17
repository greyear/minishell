#include "../../include/minishell.h"

static char	**filter_env_entries(char **env, char *name, int len, int flag)
{
	char	**temp;
	int		i;
	int		x;

	temp = allocate_temp_env(env, 0);
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
			ft_free_map(temp);
			return (NULL);
		}
		x++;
		i++;
	}
	temp[x] = NULL;
	return (temp);
}

void	rm_from_env_ex(char ***env, char *name, int len, int flag)
{
	char	**new_env;

	if (!env || !(*env))
		return;
	new_env = filter_env_entries(*env, name, len, flag);
	if (!new_env)
		return;
	ft_free_map(*env);
	*env = new_env;
}
