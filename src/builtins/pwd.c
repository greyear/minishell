#include "../../include/minishell.h"

/**
 * @brief Handles the execution of the "pwd" command.
 *
 * This function checks if the provided command is "pwd" and prints the 
 * current working directory to standard output. If `getcwd` fails, 
 * an error message is printed, and the exit status is set to 1.
 *
 * @param array The array of command arguments.
 * @param ms The shell structure containing execution state information.
 */

void	check_pwd(char **array, t_ms *ms)
{
	char	cwd[1024];

	if (!array || !*array)
		return;
	if (ft_strcmp(array[0], "pwd") != 0)
		return;
	ms->exit_status = 0;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_putstr_fd(cwd, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
	else
	{
		ms->exit_status = 1;
		perror("pwd: getcwd failed");
	}
}