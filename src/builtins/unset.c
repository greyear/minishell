/*
unset

works like this unset KEY
KEY is made of alphabets, numbers and _, number cannot be the first character
rms the KEY and its value from envp and export
if you send unset KEY= or unset KEY=VALUE, prnts ut error message and changes exit status accordingly

handle_unset(char **args, t_ms *ms)
gve args which are unset and all the KEYS needed to be removed.
for example args[0]="unset", args[1]="USER" args[2]="LOGNAME" args[3]=NULL
if one KEY s invalid like "USER=", it will cahnge exit status to 1 but still would remove the LOGNAME
and the t_ms *ms is the struct which holds the envp and exported
*/

#include "../../include/minishell.h"

static void	print_unset_error(char **args, int i, t_ms *ms)
{
	char	*err_out;

	err_out = ft_strjoin("bash: unset: '", args[i]);
	ft_putstr_fd(err_out, 2);
	free(err_out);
	ft_putstr_fd("': not a valid indentifier\n", 2);
	ms->exit_status = 1;
}

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
	while (args[i])
	{
		len = get_key_length(args[i]);
		if (len == 0)
			return;
		if (ft_strchr(args[i], '='))
			print_unset_error(args, i, ms);
		else
			process_unset_entry(args, i, ms, len);
		i++;
	}
}

