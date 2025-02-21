//#include "seela.h"
#include "../../include/minishell.h"

void	ft_free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	ft_print_err(char *cmd, int c)
{
	ft_putstr_fd("bash: ", 2);
//	ft_putstr_fd(cmd, 2);
//	ft_putstr_fd(" ", 2);
	if (cmd[0] == '/' || c == 2)
		ft_putstr_fd("No such file or directory\n", 2);
	else if (cmd[0] == '.')
		ft_putstr_fd("Permission denied\n", 2);
	else
		ft_putstr_fd("command not found\n", 2);
}
