
#include "../../include/minishell.h"

void	execve_fail(char **path, char **cmd_split, t_pipex **p)
{
	perror(cmd_split[0]);
	free(*path);
	*path = NULL;
	//clean, save error code and exit
	if (p)
		exit(CMD_EXEC);
}

void	cmd_error(char *reason, char *cmd, int exit_code, t_pipex **p)
{
	if (ft_printf(2, "%s %s\n", reason, cmd) == -1)
	{
		perror("write error");
		exit(EXIT_FAILURE);
	}
	//clean, save error code and exit
	if (p)
		exit(exit_code);
}

void	error_exit_code(char *reason, int exit_code)
{
	perror(reason);
	exit(exit_code);
}

void	error_clean_exit_code(char *reason, int exit_code, t_pipex **p)
{
	//clean, save error code and exit
	if (p)
		perror(reason);
	exit(exit_code);
}

