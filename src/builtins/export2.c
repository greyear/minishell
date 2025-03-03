#include "../../include/minishell.h"

void    sort_exported_alphaorder(t_ms *ms)
{
    int     i;
    int     j;
    char    *temp;

    i = 0;
    j = 0;
    while (ms->exported[i])
    {
        j = i + 1;
        while (ms->exported[j])
        {
            if (ft_strcmp(ms->exported[i], ms->exported[j]) > 0)
            {
                temp = ms->exported[i];
                ms->exported[i] = ms->exported[j];
                ms->exported[j] = temp;
            }
            j++;
        }
        i++;
    }
}

static void	add_to_end(char ***ex, char ***temp, char *key, int i)
{
	(*temp)[i] = ft_strdup(key);
	if (!(*temp)[i])
	{
		clean_arr(temp);
		return;
	}
	i++;
	(*temp)[i] = NULL;
	clean_arr(ex);
	*ex = *temp;
}

static void	copy_exported(char *key, char ***ex, char ***temp, int len)
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
			clean_arr(temp);
			return;
		}
        i++;
	}
	if (check == 0)
		return (add_to_end(ex, temp, key, i));
	(*temp)[i] = NULL;
	clean_arr(ex);
	*ex = *temp;
}

void	add_to_exported(char *key, t_ms *ms)
{
	char	**temp;
	int	len;
	if (check_if_valid_key(key))
		return (print_export_error(ms, key));
	len = ft_strlen(key);
	temp = allocate_temp_env(ms->exported, 2);
	copy_exported(key, &ms->exported, &temp, len);
}
