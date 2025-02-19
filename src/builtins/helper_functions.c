#include "../../include/minishell.h"

int	check_env(char *env, char *name, int len, int flag)
{
	if (ft_strncmp(name, env, len) != 0)
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

