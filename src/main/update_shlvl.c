/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_shlvl.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 14:08:01 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 14:08:03 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Handles excessively high shell levels by resetting SHLVL to 1.
 * 
 * If the shell level (`SHLVL`) exceeds a reasonable limit, this function 
 * prints a warning message (only once per execution) and resets the level to 1.
 * 
 * @param level The current shell level that is too high.
 * @param printed_warning A pointer to an integer flag indicating if the warning 
 *                        has already been printed (to avoid duplicate warnings).
 * 
 * @return A newly allocated string containing the value `"1"`, representing the 
 *         reset shell level. The caller is responsible for freeing this memory.
 * 
 * @note The function prints a warning message to standard error if the warning 
 *       has not been printed before.
 */
static char	*handle_too_high_shlvl(int level, int *printed_warning)
{
	if (!(*printed_warning))
	{
		ft_putstr_fd(OWN_ERR_MSG, STDERR_FILENO);
		ft_putstr_fd("warning: shell level (", STDERR_FILENO);
		ft_putnbr_fd(level, STDERR_FILENO);
		ft_putstr_fd(") too high, resetting to 1\n", STDERR_FILENO);
		*printed_warning = 1;
	}
	return (ft_itoa(1));
}

/**
 * @brief Increments the shell level (`SHLVL`) while handling edge cases.
 * 
 * This function increases the shell level based on its current value, ensuring 
 * that:
 * - Non-numeric or negative values reset `SHLVL` appropriately.
 * - Extremely high values trigger a warning and reset `SHLVL` to 1.
 * - If `SHLVL` exceeds `INT_MAX`, it resets to 0.
 * - If `SHLVL` reaches 999 or more, it calls `handle_too_high_shlvl` to handle 
 *   the overflow case.
 * 
 * @param shlvl_value The current `SHLVL` value as a string.
 * @param printed_warning A pointer to an integer flag indicating whether a 
 *                        warning has already been printed (to avoid redundant 
 *                        warnings).
 * 
 * @return A newly allocated string containing the new `SHLVL` value. The caller 
 *         is responsible for freeing this memory.
 */
static char	*increase_shlvl(char *shlvl_value, int *printed_warning)
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
	if (level >= INT_MAX)
		return (ft_itoa(0));
	if (level >= 999)
		return (handle_too_high_shlvl(level + 1, printed_warning));
	return (ft_itoa(level + 1));
}

/**
 * @brief Modifies the `SHLVL` environment variable by increasing its value.
 * 
 * This function searches for the `SHLVL` variable in the environment and 
 * updates it accordingly. It ensures that:
 * - `SHLVL` is properly incremented using `increase_shlvl()`.
 * - The new value is allocated dynamically and replaces the old entry.
 * - If `SHLVL` is too high, it handles the reset and warning logic.
 * - If `SHLVL` is not found, the function returns 0 (indicating no 
 *   modification was made).
 * 
 * @param env A pointer to the environment variable array (`char ***`), where 
 *            `SHLVL` will be updated in place.
 * @param printed_warning A pointer to an integer flag used to track whether a 
 *                        warning about a high `SHLVL` has already been printed.
 * 
 * @return An integer indicating the outcome:
 *         - `1` if `SHLVL` was successfully modified.
 *         - `0` if `SHLVL` was not found in the environment.
 *         - `2` if a memory allocation failure occurred.
 */
static int	modify_shlvl(char ***env, int *printed_warning)
{
	int		i;
	char	*new_value;
	char	*temp;

	i = 0;
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], "SHLVL=", 6) == 0)
		{
			new_value = increase_shlvl((*env)[i] + 6, printed_warning);
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
static void	make_args(char ***args, t_ms *ms)
{
	*args = malloc(sizeof(char *) * 3);
	if (!*args)
	{
		print_malloc_set_status(ms);
		return ;
	}
	(*args)[0] = ft_strdup("export");
	if (!(*args)[0])
	{
		clean_arr(&(*args));
		print_malloc_set_status(ms);
		return ;
	}
	(*args)[1] = ft_strdup("SHLVL=1");
	if (!(*args)[1])
	{
		clean_arr(&(*args));
		print_malloc_set_status(ms);
		return ;
	}
	(*args)[2] = NULL;
}

/**
 * @brief Updates the `SHLVL` environment variable in both the environment 
 *        and exported variables.
 * 
 * This function checks and modifies the `SHLVL` variable in the environment 
 * (`ms->envp`). If the `SHLVL` is found, it is incremented using `modify_shlvl()`. 
 * - If the `SHLVL` variable does not exist, a new variable is created and 
 *   exported using `handle_export()`.
 * - If memory allocation fails during the process, the function handles 
 *   the error and returns early.
 * - If the `SHLVL` value exceeds the maximum allowed, a warning is printed 
 *   once and the value is reset.
 * 
 * @param ms A pointer to the main shell state structure, which holds 
 *           environment and exported variables and the exit status.
 */
void	update_shlvl(t_ms *ms)
{
	char	**args;
	int		check;
	int		printed_warning;

	args = NULL;
	printed_warning = 0;
	if (ms->exit_status == MALLOC_ERR)
		return;
	check = modify_shlvl(&ms->envp, &printed_warning);
	if (check == 2)
	{
		print_malloc_set_status(ms);
		return ;
	}
	else if (check == 0)
	{
		make_args(&args, ms);
		if (ms->exit_status == MALLOC_ERR)
			return ;
		handle_export(args, ms);
		clean_arr(&args);
	}
	else
		modify_shlvl(&ms->exported, &printed_warning);
}
