#include "../../include/minishell.h"

/**
 * @brief Increases the value of the SHLVL environment variable.
 * 
 * This function takes the current value of the `SHLVL` environment variable, 
 * parses it as an integer, and increments it by 1. If the value is invalid 
 * (non-numeric or negative), it resets the value to `1` or `0` depending on 
 * the situation. The function ensures the value is valid before incrementing.
 * 
 * @param shlvl_value The current value of the `SHLVL` environment variable as 
 *                    a string.
 * 
 * @return A string representation of the new `SHLVL` value. If the value is 
 *         invalid, it returns "1" or "0".
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
		return (ft_itoa(0));
	return (ft_itoa(level + 1));
}

/**
 * @brief Modifies the `SHLVL` environment variable by incrementing its value.
 *
 * This function searches through the environment array for the `SHLVL` 
 * variable. If found, it increments the value of `SHLVL`, constructs a new 
 * value string, and updates the corresponding entry in the environment array. 
 * If memory allocation fails during this process, it returns an error code. 
 * If `SHLVL` is not found, it returns `0` to indicate no modification was made.
 *
 * @param env A pointer to the environment variable array to be modified.
 * 
 * @return `1` if `SHLVL` was successfully modified, `2` if a memory error 
 *         occurred, and `0` if `SHLVL` was not found.
 */

static int	modify_shlvl(char ***env)
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
				return (2);
			temp = ft_strjoin("SHLVL=", new_value);
			free(new_value);
			if (!temp)
				return (2);
			free((*env)[i]);
			(*env)[i] = temp;
			return (1);
		}
		i++;
	}
	return (0);
}

/**
 * @brief Allocates memory for the `export` argument array and initializes it 
 *        with default values.
 *
 * This function allocates memory for an array of strings that will represent 
 * the `export` command arguments. It sets the first element to `"export"`, 
 * the second to `"SHLVL=1"`, and ensures the array is null-terminated. If 
 * memory allocation fails at any point, it prints an error, sets the exit 
 * status to indicate a failure, and cleans up the allocated memory before 
 * returning.
 *
 * @param export A pointer to the argument array that will be created and 
 *               initialized.
 * @param ms The shell structure containing exit status and other execution 
 *           state information.
 */

static void	make_args(char ***export, t_ms *ms)
{
	*export = malloc(sizeof(char *) * 3);
	if (!*export)
	{
		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
		return ;
	}
	(*export)[0] = ft_strdup("export");
	if (!(*export)[0])
	{
		clean_arr(&(*export));
		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
		return ;
	}
	(*export)[1] = ft_strdup("SHLVL=1");
	(*export)[2] = NULL;
	if (!(*export)[1])
	{
		clean_arr(&(*export));
		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
		return ;
	}
	(*export)[2] = NULL;
}

/**
 * @brief Updates the `SHLVL` environment variable in both the `envp` and 
 *        `exported` environment lists.
 *
 * This function checks if the `envp` array exists, and if so, it creates a 
 * temporary export array, modifies the `SHLVL` variable in both the `envp` 
 * and `exported` environment lists, and handles errors during modification. 
 * If `modify_shlvl` encounters a failure, appropriate actions are taken, 
 * including printing an error message and updating the exit status. After 
 * processing, the temporary export array is cleaned up.
 *
 * @param ms The shell structure containing environment variables and exit 
 *           status information.
 */

void	update_shlvl(t_ms *ms)
{
	char	**export;
	int		check;

	if (!ms->envp)
		return ;
	modify_shlvl(&ms->envp);
	if (ms->exit_status == MALLOC_ERR)
		return ;
	make_args(&export, ms);
	if (!export)
		return ;
	check = modify_shlvl(&ms->exported);
	if (check == 0)
		handle_export(export, ms);
	if (check == 2)
	{
		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
	}
	clean_arr(&export);
}
