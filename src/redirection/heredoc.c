#include "../../include/minishell.h"

/**
 * @brief Resets the heredoc files and count for the current execution state.
 * 
 * This function allocates memory for the `heredoc_files` array, setting it to hold 100 pointers to strings, and
 * initializes the array to zero using `ft_memset`. It also resets the `heredoc_count` to 0, indicating no heredocs
 * are currently being tracked. This is useful for clearing the heredoc state before starting a new execution cycle.
 * 
 * @param ms A pointer to the `t_ms` structure that holds the global execution state, including the heredoc files
 *           and count.
 * 
 * @return This function does not return; it modifies the `ms` structure to reset the heredoc state.
 */

void	reset_heredocs(t_ms *ms)
{
	ms->heredoc_count = 0;
	ms->heredoc_files = NULL;
}

/**
 * @brief Opens the heredoc file for writing.
 * 
 * This function attempts to open a file for writing the heredoc content. It creates the file if it doesn't exist, 
 * truncates the file if it exists, and sets the file permissions to `0644`. If the file cannot be opened, 
 * an error message is printed and the program terminates.
 * 
 * @param filename The name of the file to be opened for the heredoc.
 * @param fd A pointer to the file descriptor where the file is opened.
 * 
 * @return Returns the file descriptor of the opened file. If the file cannot be opened, the program exits.
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
 * @brief Reads a line of input for the heredoc.
 * 
 * This function writes a prompt (`heredoc> `) to `stdout` to indicate that the shell is waiting for input for the heredoc. 
 * It then reads a line from the standard input (`stdin`) using `get_next_line()`. If reading the line fails, 
 * it prints an error message and terminates the program.
 * 
 * @return Returns the line read from the standard input. If an error occurs, the program exits.
 */

static char	*read_heredoc_line(int temp_fd)
{
	char	*line;
 
	write(STDOUT_FILENO, "heredoc> ", 9);
	line = get_next_line(STDIN_FILENO);
	//line = readline("heredoc> ");

	if (!line)
	{
		perror("heredoc: read error");
		close(temp_fd);
		exit(1);
	}
	return (line);
 }
 
/**
 * @brief Processes a line of input for the heredoc, expanding variables and checking for the limiter.
 * 
 * This function checks if the input line matches the heredoc delimiter (`limiter`). If it does,
 * the function frees the line and returns `1` to stop further processing. If the line does not match
 * the limiter, it performs variable expansion (unless the line is quoted), updating the line with the expanded result.
 * 
 * @param line A pointer to the line read from the input that needs to be processed.
 * @param limiter The delimiter string used to end the heredoc input.
 * @param token A pointer to the token that contains information about quotes in the line.
 * @param ms A pointer to the main shell structure, containing shell-wide information.
 * 
 * @return Returns `1` if the limiter is found, stopping further processing. Returns `0` if the line is processed successfully.
 */

static int process_heredoc_line(char **line, char *limiter, t_token *token, t_ms *ms)
{
	char	*expanded;
 
	if (ft_strncmp(*line, limiter, ft_strlen(limiter)) == 0
		&& (*line)[ft_strlen(limiter)] == '\n')
	{
		free(*line);
		return (1);
	}
	if (token->quote != SG_QUOT && token->quote != DB_QUOT)
	{
		expanded = handle_expansion(*line, ms);
		free(*line);
		*line = expanded;
	}
	return (0);
}
 
/**
 * @brief Reads input for the heredoc until the limiter is encountered.
 * 
 * This function continuously reads lines from user input until it encounters
 * the specified delimiter (limiter). Each line is written to the temporary file.
 * 
 * The process stops when the `process_heredoc_line()` function indicates that the
 * limiter has been encountered. After all the input has been processed, the
 * temporary file is closed.
 * 
 * @param temp_fd The file descriptor for the temporary heredoc file to store input.
 * @param limiter The delimiter string used to end the heredoc input.
 * @param token A pointer to the token related to the heredoc input.
 * @param ms A pointer to the main shell structure.
 * 
 * @return This function does not return a value. It terminates when the limiter is found.
 */

static void read_heredoc_input(int temp_fd, char *limiter, t_token *token, t_ms *ms)
{
	char	*line;

	while (1)
	{
		line = read_heredoc_line(temp_fd);
		if (process_heredoc_line(&line, limiter, token, ms))
			break;
		write(temp_fd, line, ft_strlen(line));
		free(line);
	}
	close(temp_fd);
}
 
/**
 * @brief Handles the heredoc functionality in the shell.
 * 
 * This function processes the heredoc input by:
 * 1. Generating a unique filename for the heredoc file.
 * 2. Storing the filename in `ms->heredoc_files`.
 * 3. Opening the heredoc file for writing and reading the input until the delimiter is encountered.
 * 4. Returning a file descriptor for reading the heredoc file.
 * 
 * If there is an error in opening the heredoc file, the function updates `ms->exit_status`
 * and returns `-1`.
 * 
 * @param ms The main shell structure, containing `heredoc_count` and `heredoc_files`.
 * @param limiter The delimiter string used to mark the end of the heredoc input.
 * @param token A pointer to the token containing heredoc-related information.
 * 
 * @return The file descriptor of the heredoc file on success, or `-1` if there was an error opening the file.
 */

int	handle_heredoc(t_ms *ms, char *limiter, t_token *token)
{
	int		temp_fd;
	int		fd;
	char	*filename;
 
	filename = generate_heredoc_filename(ms->heredoc_count);
	ms->heredoc_files[ms->heredoc_count++] = filename;
	open_heredoc_file(filename, &temp_fd);
	read_heredoc_input(temp_fd, limiter, token, ms);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		ms->exit_status = 1;
		return (-1);
	}
	return (fd);
}