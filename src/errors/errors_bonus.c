
#include "../../include/minishell.h"

/*void	execve_fail(char **path, char **cmd_split, t_pipex **p)
{
	perror(cmd_split[0]);
	free(*path);
	*path = NULL;
	//clean, save error code and exit
	if (p)
		exit(CMD_EXEC);
}*/

/*void	cmd_error(char *reason, char *cmd, int exit_code, t_pipex **p)
{
	if (ft_printf(2, "%s %s\n", reason, cmd) == -1)
	{
		perror("write error");
		exit(EXIT_FAILURE);
	}
	//clean, save error code and exit
	if (p)
		exit(exit_code);
}*/

void	error_exit_code(char *reason, int exit_code)
{
	perror(reason);
	exit(exit_code);
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
