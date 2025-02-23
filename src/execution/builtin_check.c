
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

//needs to be checked by Seela mb
//mb all the arguments should be in the same order? like cmd 1st, ms 2nd... and names -handle everywhere instead of check
void	handle_builtin(t_cmd *cmd, t_ms *ms)
{
	if (ft_strcmp(cmd->name, "echo") == 0)
		handle_echo(cmd->args, ms);
	else if (ft_strcmp(cmd->name, "cd") == 0)
		handle_cd(cmd->args, ms);
	else if (ft_strcmp(cmd->name, "pwd") == 0)
		check_pwd(cmd->args, ms);
	else if (ft_strcmp(cmd->name, "export") == 0)
		handle_export(cmd->args, ms);
	else if (ft_strcmp(cmd->name, "unset") == 0)
		handle_unset(cmd->args, ms);
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