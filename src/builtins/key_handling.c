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

void	process_unset_entry(char **args, int i, t_ms *ms, int len)
{
	char	*name;

	name = extract_key(args[i], len);
	if (!name)
		return;
	if (check_if_valid_key(name, args, i, ms) == 0)
	{
		rm_from_env_ex(&ms->exported, name, len, 1);
		rm_from_env_ex(&ms->envp, name, len, 0);
	}
	free(name);
}
