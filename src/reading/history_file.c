/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 14:20:18 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 14:20:20 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//mallocs checked

//mallocs checked

#include "../../include/minishell.h"

/**
 * @brief Reads command history from a file and stores it in ms structure.
 * 
 * This function reads up to `HISTORY_SIZE` lines from the given file descriptor 
 * `fd` and stores them in the `ms->history` array. Each line is also added to 
 * the readline history using `add_history()`.
 * 
 * @param fd File descriptor of the history file.
 * @param ms Pointer to the shell structure where the history is stored.
 * 
 * @note If memory allocation fails, the function stops reading further lines.
 */
static void	read_from_history_file(int fd, t_ms *ms) //fill_history +
{
	char	*buf;
	char	*trimmed;

	while (1)
	{
		buf = get_next_line(fd);
		if (!buf)
			return ;
		trimmed = ft_strtrim(buf, "\n");
		if (!trimmed)
		{
			free(buf);
			print_malloc_set_status(ms);
			return ;
		}
		add_history(trimmed);
		fill_ms_history_line(trimmed, ms);
		free(buf);
		free(trimmed);
	}
}

/**
 * @brief Opens the history file for reading and puts its contents into the 
 *        shell structure.
 * 
 * This function attempts to open the history file in read-only mode. If 
 * the file does not exist, it creates an empty history file with 
 * appropriate permissions. If the file exists, its content is read and 
 * stored in the shell's history structure.
 * 
 * @param ms Pointer to the shell structure where the history will be stored.
 * 
 * @return SUCCESS on successful execution.
 * 
 * @note If the file does not exist, it is created but remains empty.
 */
t_bool	open_read_history_file(t_ms *ms) //fetch_history_file +
{
	int	fd;

	fd = open(HISTORY_FILE, O_RDONLY);
	if (fd < 0)
	{
		fd = open(HISTORY_FILE, O_CREAT, 0644);
		close(fd);
		return (0);
	}
	read_from_history_file(fd, ms);
	close(fd);
	return (0);
}

/**
 * @brief Writes the shell history to the history file.
 * 
 * This function opens the history file in truncate mode and writes the 
 * stored history from the shell structure into the file. The history is 
 * written line by line.
 * 
 * @param ms Pointer to the shell structure containing the command history.
 * 
 * @note The function overwrites the existing history file.
 */
void	write_to_history_file(t_ms *ms) //add_to_history_file +
{
	int	fd;
	int	i;

	fd = open(HISTORY_FILE, O_TRUNC | O_RDWR);
	if (fd < 0)
		return ;
	i = 0;
	while (i < HISTORY_SIZE)
	{
		if (ms->history[ms->history_num])
			ft_putendl_fd(ms->history[ms->history_num], fd);
		if (ms->history_num == HISTORY_SIZE - 1)
			ms->history_num = 0;
		else
			ms->history_num++;
		i++;
	}
	close(fd);
}
