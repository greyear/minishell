/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 14:23:06 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 14:23:07 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Handles the parent process logic after a heredoc operation.
 * 
 * This function waits for the child process (that reads input for the heredoc) 
 * to complete. It checks the exit status of the child process, updating the 
 * shell's exit status accordingly. If the child process exits with a status 
 * of 130 (indicating a signal interruption, like `Ctrl+C`), it sets a signal 
 * flag. After the child process exits, the function attempts to open the 
 * heredoc file for reading and returns the file descriptor. If opening the 
 * file fails, the shell's exit status is set to 1.
 * 
 * @param ms A pointer to the `t_ms` structure, which holds the shell's state.
 * @param pid The process ID of the child process that handles the heredoc input.
 * @param filename The name of the file containing the heredoc content.
 * 
 * @return The file descriptor for the heredoc file if successful, or -1 if 
 *         an error occurs.
 */
static int	handle_heredoc_parent(t_ms *ms, pid_t pid, char *filename)
{
	int		status;
	int		fd;

	signal_mode(IGNORE);
	waitpid(pid, &status, 0);
	signal_mode(DEFAULT);
	if (WIFEXITED(status))
	{
		ms->exit_status = WEXITSTATUS(status);
		if (ms->exit_status == 130)
		{
			g_sgnl = SIGINT;
			return (SIGNAL_HEREDOC);
		}
	}
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		ms->exit_status = 130;
		return (SIGNAL_HEREDOC);
	}
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		ms->exit_status = 1;
	return (fd);
}

/**
 * @brief Processes a line of input for the heredoc, expanding variables 
 * and checking for the limiter.
 * 
 * This function checks if the input line matches the heredoc delimiter 
 * (`limiter`). If it does, the function frees the line and returns `1` 
 * to stop further processing. If the line does not match the limiter, 
 * it performs variable expansion (unless the line is quoted), updating 
 * the line with the expanded result.
 * 
 * @param line A pointer to the line read from the input that needs to be 
 * processed.
 * @param lim The delimiter string used to end the heredoc input.
 * @param token A pointer to the token that contains information about 
 * quotes in the line.
 * @param ms A pointer to the main shell structure, containing shell-wide 
 * information.
 * 
 * @return Returns `1` if the limiter is found, stopping further processing. 
 * Returns `0` if the line is processed successfully.
 */
static int	process_line(char **line, char *lim, t_token *token, t_ms *ms)
{
	char		*expanded;
	t_expand	*exp;

	exp = exp_init(ms);
	if (ft_strcmp(*line, lim) == 0)
	{
		free(*line);
		free(exp);
		return (1);
	}
	if (token->quote != SG_QUOT && token->quote != DB_QUOT)
	{
		exp->data = *line;
		expanded = handle_expansion(exp, ms);
		free(*line);
		*line = expanded;
	}
	free(exp);
	return (0);
}

/**
 * @brief Handles user input for a heredoc and writes it to a temporary file.
 * 
 * This function reads input line by line until the limiter is encountered or an 
 * interruption (Ctrl+C) occurs. Each line is processed and written to the 
 * temporary file unless it matches the limiter.
 * 
 * @param fd  The file descriptor of the temporary file to store heredoc
 *                 input.
 * @param lim  The delimiter string that marks the end of heredoc input.
 * @param token    A pointer to the token structure related to the heredoc.
 * @param ms       A pointer to the main shell structure.
 * 
 * @note If Ctrl+C is pressed, the function exits with status 130.
 * @note If EOF (Ctrl+D) is encountered, an appropriate message is printed.
 */
static void	handle_heredoc_input(int fd, char *lim, t_token *token, t_ms *ms)
{
	char	*line;

	while (1)
	{
		signal_mode(HEREDOC_MODE);
		line = readline("> ");
		signal_mode(IGNORE);
		if (g_sgnl == SIGINT)
		{
			free(line);
			close(fd);
			exit(130);
		}
		if (!line)
		{
			print_heredoc_ctrl_d(lim);
			return ;
		}
		if (process_line(&line, lim, token, ms))
			break ;
		ft_putendl_fd(line, fd);
		free(line);
	}
}

/**
 * @brief Reads input for a heredoc and writes it to a temporary file.
 * 
 * This function opens a temporary file with write permissions and truncates it 
 * if it already exists. It then handles user input until the limiter is 
 * encountered or an interruption occurs. The input is written to the file.
 * 
 * @param filename The name of the temporary file where the heredoc input 
 *                 will be stored.
 * @param lim  The delimiter string indicating the end of heredoc input.
 * @param token    A pointer to the token structure related to the heredoc.
 * @param ms       A pointer to the main shell structure.
 * 
 * @note If the file cannot be opened, an error message is printed and 
 *       the process exits.
 */
static void	read_input(char *filename, char *lim, t_token *token, t_ms *ms)
{
	int		temp_fd;

	temp_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (temp_fd < 0)
	{
		perror("heredoc: open failed");
		exit(1);
	}
	handle_heredoc_input(temp_fd, lim, token, ms);
	close(temp_fd);
	exit(0);
}

/**
 * @brief Handles the heredoc process by creating a child process to read input.
 * 
 * This function generates a filename for the heredoc file and creates a child 
 * process to handle reading the input for the heredoc. The child process writes 
 * the input to the temporary file. The parent process waits for the child to 
 * complete and returns the file descriptor of the heredoc file. If any error 
 * occurs during the creation of the child process or file handling, it updates 
 * the shell's exit status accordingly.
 * 
 * @param ms A pointer to the `t_ms` structure, which holds the shell's state.
 * @param lim The string (delimiter) that marks the end of the heredoc input.
 * @param token A pointer to the token structure for processing the heredoc 
 *              input.
 * 
 * @return The file descriptor for the heredoc file if successful, or -1 if an 
 *         error occurs.
 */
int	handle_heredoc(t_ms *ms, char *lim, t_token *token)
{
	char	*filename;
	pid_t	pid;

	filename = generate_filename(ms->heredoc_count, ms);
	if (!filename)
		return (-1);
	ms->heredoc_files[ms->heredoc_count++] = filename;
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		ms->exit_status = SYSTEM_ERR;
		return (-1);
	}
	else if (pid == 0)
	{
		signal_mode(HEREDOC_MODE);
		read_input(filename, lim, token, ms);
	}
	return (handle_heredoc_parent(ms, pid, filename));
}
