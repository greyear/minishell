#include "../../include/minishell.h"

static int	should_skip(char *env, char *name, int len, int flag)
{
	if (ft_strncmp(name, env, len) != 0)
		return (0);
	if (flag == 0 && (env[len] && env[len] == '='))
		return (1);
	if (flag == 1 && (env[len] == '=' || env[len] == '\0'))
		return (1);
	return (0);
}

static char	**allocate_temp_env(char **env)
{
	char	**temp;
	int	i;

	i = 0;
	while (env[i])
		i++;
	if (i == 0)
		return (NULL);
	temp = malloc(sizeof(char *) * i);
	if (!temp)
		return (NULL);
	return (temp);
}

static char	**filter_env_entries(char **env, char *name, int len, int flag)
{
	char	**temp;
	int		i;
	int		x;

	temp = allocate_temp_env(env);
	if (!temp)
		return (NULL);
	i = 0;
	x = 0;
	while (env[i])
	{
		if (should_skip(env[i], name, len, flag))
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
