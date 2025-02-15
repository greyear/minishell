#include "../../include/minishell.h"


char	**make_args(char *cmd)
{
	char	**args;

	args = malloc(sizeof(char*) * 3);
	if (!args)
		return (NULL);
	args[0] = ft_strdup(cmd);
	if (!args[0])
	{
		ft_free_map(args);
		return (NULL);
	}
	args[1] = ft_strdup("OLDPWD");
	if (!args[1])
	{
		ft_free_map(args);
		return (NULL);
	}	
	args[2] = NULL;
	return (args);
}

//We need to remove OLDPWD in the beginning like in bash!, and add it to exported without an value!

int	handle_oldpwd(t_ms *ms)
{
	char	**args;

	args = make_args("unset");
	if (!args)
		return (1);
	handle_unset(args, ms);
	ft_free_map(args);
	args = make_args("export");
	if (!args)
		return (1);
	handle_export(args, ms);
	ft_free_map(args);
	return (0);
}

t_ms	*initialize_struct(char **envp)
{
	t_ms *ms;

	ms = malloc(sizeof(t_ms));
	if (!ms)
		return (NULL);
	ms->exit_status = 0;
	ms->envp = copy_map(envp);
	ms->exported = copy_map(envp);
	ms->check_oldpwd = 1;
	if (handle_oldpwd(ms) == 1)
		return (NULL);
	return (ms);
}
