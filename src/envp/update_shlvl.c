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

/**
 * @brief Updates the SHLVL environment variable.
 * 
 * This function searches through the environment variable array for the `SHLVL` variable, 
 * increments its value using the `increase_shlvl` function, and updates the environment with the new value.
 * If the `SHLVL` variable is not found, the function returns 0, indicating no update was made.
 * 
 * @param env A pointer to the environment variable array.
 * 
 * @return Returns 1 if the `SHLVL` variable was found and updated. Returns 0 if the variable was not found.
 */

static int	update_shlvl(char ***env)
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
				return (0);
			temp = ft_strjoin("SHLVL=", new_value);
			free(new_value);
			if (!temp)
				return(0);
			free((*env)[i]);
			(*env)[i] = temp;
			return (1);
        }
        i++;
    }
	return (0);
}

/**
 * @brief Allocates and initializes an array of strings for the export command.
 * 
 * This function dynamically allocates memory for an array of strings, initializing it with 
 * the `export` command and the `SHLVL=1` variable. The function sets the exit status to 1 if 
 * any allocation fails, and cleans up any allocated memory before returning.
 * 
 * @param export A pointer to the array of strings to be populated with the export command and environment variable.
 * @param ms A pointer to the main program state, which is used to track the exit status.
 * 
 * @return This function does not return a value. It modifies the `export` array and the `exit_status` of the program.
 */

static void	make_args(char ***export, t_ms *ms)
{
	*export = malloc(sizeof(char *) * 3);
	if (!*export)
	{
		ms->exit_status = 1;
		return;
	}
	(*export)[0] = ft_strdup("export");
	if (!(*export)[0])
	{
		clean_arr(&(*export));
		ms->exit_status = 1;
		return;
	}
	(*export)[1] = ft_strdup("SHLVL=1");
	(*export)[2] = NULL;
	if (!(*export)[1])
	{
		clean_arr(&(*export));
		ms->exit_status = 1;
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
	update_shlvl(&ms->envp);
	if (!update_shlvl(&ms->exported))
		handle_export(export, ms);
	clean_arr(&export);
}
