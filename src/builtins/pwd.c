#include "../../include/minishell.h"

/**
 * @brief Handles the execution of the "pwd" command.
 *
 * This function prints the current working directory to standard output.
 * If `getcwd` fails, an error message is printed, and the exit status
 * is set to 1.
 *
 * @param array The array of command arguments.
 * @param ms The shell structure containing execution state information.
 */
void	check_pwd(t_ms *ms)
{
	char	cwd[1024];

	ms->exit_status = 0;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_putstr_fd(cwd, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		return;
	}
	if (ms->pwd)
	{
		ft_putstr_fd(ms->pwd, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		return;
	}
	else
	{
		perror("pwd: getcwd failed");
		ms->exit_status = 1;
	}
}
