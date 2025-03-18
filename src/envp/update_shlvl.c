#include "../../include/minishell.h"

/**
 * @brief Increases the value of the SHLVL environment variable.
 * 
 * This function takes the current value of the `SHLVL` environment variable, parses it as an integer,
 * and increments it by 1. If the value is invalid (non-numeric or negative), it resets the value to `1` 
 * or `0` depending on the situation. The function ensures the value is valid before incrementing.
 * 
 * @param shlvl_value The current value of the `SHLVL` environment variable as a string.
 * 
 * @return A string representation of the new `SHLVL` value. If the value is invalid, it returns "1" or "0".
 */

static char	*increase_shlvl(char *shlvl_value)
{
	int		level;
	int		i;
	
	i = 0;
	level = ft_atoi(shlvl_value);
	while (shlvl_value[i])
	{
		if (shlvl_value[0] != '-' && !ft_isdigit(shlvl_value[0]))
			return (ft_itoa(1));
		if (i != 0 && !ft_isdigit(shlvl_value[i]))
			return (ft_itoa(1));
		i++;
	}
	if (level < 0)
		return ft_itoa(0);
	return (ft_itoa(level + 1));
}


static int	update_shlvl(char ***env, t_ms *ms)
{
	int		i;
	char	*new_value;
	char	*temp;
	
	i = 0;
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], "SHLVL=", 6) == 0)
		{
		   	new_value = increase_shlvl((*env)[i] + 6);
			if (!new_value)
			{
				print_malloc_error();
				ms->exit_status = MALLOC_ERR;
				return (0);
			}
			temp = ft_strjoin("SHLVL=", new_value);
			free(new_value);
			if (!temp)
			{
				print_malloc_error();
				ms->exit_status = MALLOC_ERR;
				return(0);
			}
			free((*env)[i]);
			(*env)[i] = temp;
			return (1);
		}
		i++;
	}
	return (0);
}


static void	make_args(char ***export, t_ms *ms)
{
	*export = malloc(sizeof(char *) * 3);
	if (!*export)
	{
		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
		return;
	}
	(*export)[0] = ft_strdup("export");
	if (!(*export)[0])
	{
		clean_arr(&(*export));
		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
		return;
	}
	(*export)[1] = ft_strdup("SHLVL=1");
	(*export)[2] = NULL;
	if (!(*export)[1])
	{
		clean_arr(&(*export));
		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
		return;
	}
	(*export)[2] = NULL;
}

/**
 * @brief Checks and updates the SHLVL environment variable.
 * 
 * This function checks and updates the `SHLVL` variable in the program's environment.
 * It creates an array of arguments for the `export` command, updates the `SHLVL` value
 * in both the environment and exported environment, and if necessary, handles the export
 * operation. The function also handles memory cleanup after operations.
 * 
 * @param ms A pointer to the main program state, which contains the environment and exported variables.
 * 
 * @return This function does not return a value. It modifies the `envp` and `exported` variables in the `ms` structure.
 */

void	check_shlvl(t_ms *ms)
{
	char	**export;

	if (!ms->envp)
		return;
	make_args(&export, ms);
	if (!export)
		return;
	update_shlvl(&ms->envp, ms);
	if (ms->exit_status == MALLOC_ERR)
		return;
	if (!update_shlvl(&ms->exported, ms))
	{
		if (ms->exit_status != MALLOC_ERR)
		handle_export(export, ms);
	}
	clean_arr(&export);
}
