#include "../../include/minishell.h"

static void	process_unset_entry(char **args, int i, t_ms *ms, int len)
{
	char	*name;

	name = extract_key(args[i], len);
	if (!name)
		return;
	if (check_if_valid_key(name) == 1)
	{
		print_unset_error(args, i, ms);
		free(name);
		return;
	}
	rm_from_env_ex(&ms->exported, name, len, 1);
	rm_from_env_ex(&ms->envp, name, len, 0);
	free(name);
}

void	handle_unset(char **args, t_ms *ms)
{
	int	len;
	int	i;

	i = 1;
	ms->exit_status = 0;
	if (!args || !*args)
		return;
	if (ft_strcmp(args[0], "unset") != 0)
		return;
	while (args[i])
	{
		len = get_key_length(args[i]);
		if (len == 0 || ft_strchr(args[i], '=')
			|| args[i][0] == '=')
			print_unset_error(args, i, ms);
		else
			process_unset_entry(args, i, ms, len);
		i++;
	}
}