#include "../../include/minishell.h"

void	add_to_end(char ***ex, char ***temp, char *key, int i)
{
	(*temp)[i] = ft_strdup(key);
	if (!(*temp)[i])
	{
		ft_free_map(*temp);
		return;
	}
	i++;
	(*temp)[i] = NULL;
	ft_free_map(*ex);
	*ex = *temp;
}

void	copy_exported(char *key, char ***ex, char ***temp, int len)
{
	int	i;
	int	check;

	i = 0;
	check = 0;
	while ((*ex)[i])
	{
		if (check_env((*ex)[i], key, len, 1))
			check = 1;
		(*temp)[i] = ft_strdup((*ex)[i]);
		if (!(*temp)[i])
		{
			printf("error");
			ft_free_map(*temp);
			return;
		}
        i++;
	}
	if (check == 0)
		return (add_to_end(ex, temp, key, i));
	(*temp)[i] = NULL;
	ft_free_map(*ex);
	*ex = *temp;
}

void	add_to_exported(char *key, t_ms *ms)
{
	char	**temp;
	int	len;
	if (check_if_valid_key(key))
		return (print_error3(ms, key));
	len = ft_strlen(key);
	temp = allocate_temp_env(ms->exported, 2);
	copy_exported(key, &ms->exported, &temp, len);
}
