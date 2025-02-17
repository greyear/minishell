#include "../../include/minishell.h"

int	get_key_length(char *arg)
{
	int		len;

	len = 0;
	while (arg[len] && arg[len] != '=')
		len++;
	return (len);
}

char	*extract_key(char *arg, int len)
{
	char	*name;

	name = malloc(sizeof(char) * (len + 1));
	if (!name)
		return (NULL);
	ft_strncpy(name, arg, len);
	name[len] = '\0';
	return (name);
}

int		check_if_valid_key(char *name)
{
	int		x;

	x = 0;
	if (!name || !*name)
		return (1);
	if (name[0] >= '0' && name[0] <= '9')
		return (1);
	while (name[x])
	{
		if (!ft_isalnum(name[x]) && name[x] != '_')
			return (1);
		x++;
	}
	return (0);
}
