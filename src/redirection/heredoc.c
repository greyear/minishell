#include "../../include/minishell.h"

/**
 * @brief Opens a file for a heredoc and returns its file descriptor.
 * 
 * This function attempts to open a file specified by `filename` for writing. If the file does not exist, it is created.
 * If the file already exists, its content is truncated. The file is opened with the permissions `0644` (readable and
 * writable by the owner, and readable by others). If the file cannot be opened, an error message is printed, and the
 * program exits with a status of 1.
 * 
 * @param filename The name of the file to be opened or created for the heredoc.
 * @param fd A pointer to an integer where the file descriptor will be stored.
 *           This value will be updated with the file descriptor of the opened file.
 * 
 * @return The file descriptor of the opened file.
 *         If the file cannot be opened, the program exits.
 */

static int	open_heredoc_file(char *filename, int *fd)
{
	*fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd < 0)
	{
		perror("heredoc: open failed");
		exit(1);
	}
	return (*fd);
}

/**
 * @brief Reads input for a heredoc and writes it to a temporary file.
 * 
 * This function continuously prompts the user for input with the prompt "heredoc> ". It reads each line from the 
 * standard input and writes it to a temporary file until the input matches the specified `limiter`. Once the limiter 
 * is detected, the input is stopped, the line containing the limiter is freed, and the file is closed.
 * 
 * If there is an error reading from standard input, an error message is printed, and the program exits.
 * 
 * @param temp_fd The file descriptor of the temporary file where the heredoc content is written.
 * @param limiter A string that acts as a delimiter to stop reading input. Once the input matches the limiter, the function exits.
 */

static void	read_heredoc_input(int temp_fd, char *limiter)
{
	char *line;

	while (1)
	{
		write(STDOUT_FILENO, "heredoc> ", 9);
		line = get_next_line(STDIN_FILENO);
		if (!line) //clean_gnl?
		{
			
			perror("heredoc: read error");
			close(temp_fd);
			exit(1);
		}
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
			&& line[ft_strlen(limiter)] == '\n')
		{
			free(line);
			break;
		}
		write(temp_fd, line, ft_strlen(line));
		free(line);
	}
	close(temp_fd);
}

/**
 * @brief Handles the creation and reading of a heredoc file.
 * 
 * This function generates a unique filename for the heredoc file, opens the file for writing, and reads input from
 * the user until the input matches the specified `limiter`. The input is written to the temporary file, and once the
 * limiter is reached, the file is closed. Then, the function opens the file in read-only mode and returns the file 
 * descriptor for the heredoc file.
 * 
 * If there is an error opening the file for reading, the function sets the exit status to 1 and returns -1.
 * 
 * @param ms A pointer to the main structure containing the heredoc files and other relevant data.
 * @param limiter A string that acts as a delimiter to stop reading input. Once the input matches the limiter, the function exits.
 * 
 * @return The file descriptor of the heredoc file if successful, or -1 if an error occurs.
 */

int	handle_heredoc(t_ms *ms, char *limiter)
{
	int temp_fd;
	int fd;
	char *filename;

	filename = generate_heredoc_filename(ms->heredoc_count);
	ms->heredoc_files[ms->heredoc_count++] = filename;
	open_heredoc_file(filename, &temp_fd);
	read_heredoc_input(temp_fd, limiter);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		ms->exit_status = 1;
		return (-1);
	}
	return (fd);
}