/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 14:20:26 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 14:20:29 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Initializes the history array by setting all its lines to NULL.
 * 
 * This function ensures that all elements in the history array are 
 * initialized to NULL, preventing potential undefined behavior.
 * 
 * @param history A pointer to the history array.
 */
void	default_history(char **history)
{
	int	i;

	i = 0;
	while (i < HISTORY_SIZE)
	{
		history[i] = NULL;
		i++;
	}
}

/**
 * @brief Stores a command line into the ms history field.
 * 
 * This function saves the given command line into the history array 
 * within the `t_ms` structure. If the current history slot is occupied, 
 * it frees the memory before storing the new line. The function ensures 
 * a circular buffer behavior by resetting the index when it reaches the 
 * limit.
 * 
 * @param line The command line to be stored in history.
 * @param ms A pointer to the shell structure containing the history array.
 */
void	fill_ms_history_line(char *line, t_ms *ms)
{
	if (ms->history[ms->history_num])
		free(ms->history[ms->history_num]);
	ms->history[ms->history_num] = ft_strdup(line);
	if (!ms->history[ms->history_num])
	{
		print_malloc_set_status(ms);
		return ;
	}
	if (ms->history_num == HISTORY_SIZE - 1)
		ms->history_num = 0;
	else
		ms->history_num++;
}

/**
 * @brief Adds a command line to the shell's history.
 * 
 * This function checks if the given line is not empty and, if so, 
 * adds it to both the readline history and the ms field history array.
 * 
 * @param line The command line to be added to history.
 * @param ms A pointer to the shell structure containing the history.
 */
void	add_line_to_history(char *line, t_ms *ms)
{
	if (line[0] != '\0')
	{
		add_history(line);
		if (ms->history_file == true)
			fill_ms_history_line(line, ms);
	}
}

/**
 * @brief Frees the allocated memory for the ms field history array.
 * 
 * This function iterates through the history array in the `t_ms` structure 
 * and frees each allocated string, setting the corresponding pointers to NULL.
 * 
 * @param ms A pointer to the shell structure containing the history.
 */
void	clean_ms_history(t_ms *ms)
{
	int	i;

	i = 0;
	while (i < HISTORY_SIZE)
	{
		if (ms->history[i])
		{
			free(ms->history[i]);
			ms->history[i] = NULL;
		}
		i++;
	}
}

/**
 * @brief Handles the exit process for the shell's history.
 * 
 * This function ensures that the command history is properly saved 
 * before the shell exits. It writes the current history to a file 
 * and then clears the in-memory history to free resources.
 * 
 * @param ms A pointer to the shell structure containing the history.
 */
void	history_exit(t_ms *ms)
{
	write_to_history_file(ms);
	clean_ms_history(ms);
}
