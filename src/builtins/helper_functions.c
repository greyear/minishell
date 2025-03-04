#include "../../include/minishell.h"

int	check_env(char *env, char *key, int len, int flag)
{
	if (ft_strncmp(key, env, len) != 0)
		return (0);
	if (flag == 0 && (env[len] && env[len] == '='))
		return (1);
	if (flag == 1 && (env[len] == '\0'
		|| (env[len] && env[len] == '=')))
		return (1);
	return (0);
}

char	**allocate_temp_env(char **env, int x)
{
	char	**temp;
	int	i;

	i = 0;
	while (env[i])
		i++;
	if (i == 0)
		return (NULL);
	temp = malloc(sizeof(char *) * (i + x));
	if (!temp)
		return (NULL);
	return (temp);
}

void    print_array(char **a)
{
	int	i;

	i = 0;
	while (a[i])
	{
		printf("%s\n", a[i]);
		i++;
	}
}

char	**copy_map(char **original_map)
{
	char	**new_map;
	int		i;

	i = 0;
	while (original_map[i])
		i++;
	new_map = malloc(sizeof(char *) * (i + 1));
	if (!new_map)
		return (NULL);
	i = 0;
	while (original_map[i])
	{
		new_map[i] = ft_strdup(original_map[i]);
		if (!new_map[i])
		{
			//clean_arr(new_map);
			clean_arr(&(new_map));
			return (NULL);
		}
		i++;
	}
	new_map[i] = NULL;
	return (new_map);
}