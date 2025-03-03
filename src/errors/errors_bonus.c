
#include "../../include/minishell.h"

void	print_system_error(t_print reason)
{
	ft_putstr_fd(OWN_ERR_MSG, STDERR_FILENO);
	if (reason == HIST_ERR)
		ft_putendl_fd("Reading from history file failed", STDERR_FILENO);
}

void	print_syntax_error(char *text)
{
	ft_putstr_fd(OWN_ERR_MSG, STDERR_FILENO);

	if (*text == NULL_TERM)
		text = "newline";
	ft_putstr_fd(": syntax error near unexpected token `", STDERR_FILENO);
	ft_putstr_fd(text, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
}

void	print_file_error(char *file, t_print reason)
{
	ft_putstr_fd(OWN_ERR_MSG, STDERR_FILENO);
	if (reason == HERED_ERR)
	{
		//put msg
	}
	else
		ft_putstr_fd(file, STDERR_FILENO);
	if (reason == NO_FILE)
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	if (reason == PERM_DEN)
		ft_putendl_fd(": Permission denied", STDERR_FILENO);
	if (reason == AMBIG)
		ft_putendl_fd(": ambiguous redirect", STDERR_FILENO);
}

void	print_cmd_error(char *cmd, int c)
{
	ft_putstr_fd(OWN_ERR_MSG, 2);
	ft_putstr_fd(cmd, 2);
	if (cmd[0] == '/' || c == 2)
		ft_putstr_fd(": No such file or directory\n", 2);
	else if (cmd[0] == '.')
		ft_putstr_fd(": Permission denied\n", 2);
	else
		ft_putstr_fd(": command not found\n", 2);
}

void	print_unset_error(char **args, int i, t_ms *ms)
{
	ft_putstr_fd(OWN_ERR_MSG, 2);
	ft_putstr_fd("unset: '", 2);
	ft_putstr_fd(args[i], 2);
	ft_putstr_fd("': not a valid indentifier\n", 2);
	ms->exit_status = 1;
}

void	print_export_error(t_ms *ms, char *arg)
{
    ft_putstr_fd(OWN_ERR_MSG, 2);
	ft_putstr_fd("export: '", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	ms->exit_status = 1;
}

void	print_cd_error(char *target_dir)
{
	ft_putstr_fd(OWN_ERR_MSG, 2);
	ft_putstr_fd("cd: ", 2);
	if (target_dir && *target_dir)
		ft_putstr_fd(target_dir, 2);
	else
		ft_putstr_fd(" ", 2);
	ft_putstr_fd(": No such file or directory\n", 2);
}