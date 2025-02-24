
#include "../../include/minishell.h"

int	is_builtin(t_cmd *cmd)
{
	if ((ft_strcmp(cmd->name, "echo") == 0) || \
		(ft_strcmp(cmd->name, "cd") == 0) || \
		(ft_strcmp(cmd->name, "pwd") == 0) || \
		(ft_strcmp(cmd->name, "export") == 0) || \
		(ft_strcmp(cmd->name, "unset") == 0) || \
		(ft_strcmp(cmd->name, "env") == 0) || \
		(ft_strcmp(cmd->name, "exit") == 0))
		return (1);
	return (0);
}

void	handle_builtin(t_cmd *cmd, t_ms *ms, int in_child)
{
	if (ft_strcmp(cmd->name, "echo") == 0)
		handle_echo(cmd->args, ms);
	else if (ft_strcmp(cmd->name, "cd") == 0)
		handle_cd(cmd->args, ms);
	else if (ft_strcmp(cmd->name, "pwd") == 0)
		check_pwd(cmd->args, ms);
	else if (ft_strcmp(cmd->name, "export") == 0)
	{
		if (in_child)
			handle_export(cmd->args, NULL);
		else
			handle_export(cmd->args, ms);
	}
	else if (ft_strcmp(cmd->name, "unset") == 0)
	{
		if (!in_child)
			handle_unset(cmd->args, ms);
	}
	else if (ft_strcmp(cmd->name, "env") == 0)
		handle_env(cmd->args, ms);
	else if (ft_strcmp(cmd->name, "exit") == 0)
		check_exit(cmd->args, ms);
}

int	if_children_needed(t_cmd *cmd)
{
	if (ft_strcmp(cmd->name, "cd") == 0 || \
		ft_strcmp(cmd->name, "exit") == 0 || \
		ft_strcmp(cmd->name, "unset") == 0 || \
		(ft_strcmp(cmd->name, "export") == 0 && cmd->args[1]))
		return (false);
	return (true);
}