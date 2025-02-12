#include "seela.h"

void	print_cd_error(char *target_dir)
{
	ft_putstr_fd("bash: cd: ", 2);
	if (target_dir && *target_dir)
		ft_putstr_fd(target_dir, 2);
	else
		ft_putstr_fd(" ", 2);
	ft_putstr_fd(": No such file or directory\n", 2);
}
