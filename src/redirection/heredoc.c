#include "../../include/minishell.h"

/**
 * @brief Opens the heredoc file for writing.
 * 
 * This function attempts to open a file for writing the heredoc content. 
 * It creates the file if it doesn't exist, truncates the file if it exists, 
 * and sets the file permissions to `0644`. If the file cannot be opened, 
 * an error message is printed and the program terminates.
 * 
 * @param filename The name of the file to be opened for the heredoc.
 * @param fd A pointer to the file descriptor where the file is opened.
 * 
 * @return Returns the file descriptor of the opened file. If the file cannot 
 * be opened, the program exits.
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
 * @brief Reads a line of input for a heredoc.
 * 
 * This function handles reading a line of input from the user for a heredoc.
 * It uses `readline` to get input and processes signals to manage the heredoc 
 * mode (e.g., handle `Ctrl+D` for EOF). If `readline` returns `NULL`, it 
 * indicates an EOF signal, and the heredoc is closed with a message printed 
 * to indicate the EOF. The function will then close the temporary file 
 * descriptor and exit the program.
 * 
 * @param temp_fd The temporary file descriptor to be closed if EOF is 
 * encountered.
 * @param limiter The string to be printed when `Ctrl+D` is pressed, 
 * indicating the EOF condition.
 * 
 * @return A pointer to the line of input read from the user.
 */
static char	*read_line(int temp_fd, char *limiter)
{
	char	*line;

	signal_mode(HEREDOC_MODE);
	line = readline("> ");
	signal_mode(IGNORE);
	if (!line)
	{
		print_heredoc_ctrl_d(limiter);
		close(temp_fd);
		exit(0);
	}
	return (line);
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
 * @param limiter The delimiter string used to end the heredoc input.
 * @param token A pointer to the token that contains information about 
 * quotes in the line.
 * @param ms A pointer to the main shell structure, containing shell-wide 
 * information.
 * 
 * @return Returns `1` if the limiter is found, stopping further processing. 
 * Returns `0` if the line is processed successfully.
 */
static int	process_line(char **line, char *limiter, t_token *token, t_ms *ms)
{
	char		*expanded;
	t_expand	*exp;

	exp = exp_init();
	if (ft_strcmp(*line, limiter) == 0)
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
 * @brief Reads input for the heredoc until the limiter is encountered.
 * 
 * This function continuously reads lines from user input until it encounters
 * the specified delimiter (limiter). Each line is written to the temporary 
 * file.
 * 
 * The process stops when the `process_heredoc_line()` function indicates 
 * that the limiter has been encountered. After all the input has been 
 * processed, the temporary file is closed.
 * 
 * @param temp_fd The file descriptor for the temporary heredoc file to 
 * store input.
 * @param limiter The delimiter string used to end the heredoc input.
 * @param token A pointer to the token related to the heredoc input.
 * @param ms A pointer to the main shell structure.
 * 
 * @return This function does not return a value. It terminates when the 
 * limiter is found.
 */
static void	read_input(int temp_fd, char *limiter, t_token *token, t_ms *ms)
{
	char	*line;

	while (1)
	{
		line = read_line(temp_fd, limiter);
		if (process_line(&line, limiter, token, ms))
			break ;
		ft_putendl_fd(line, temp_fd);
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
 * 3. Opening the heredoc file for writing and reading the input until the 
 * delimiter is encountered.
 * 4. Returning a file descriptor for reading the heredoc file.
 * 
 * If there is an error in opening the heredoc file, the function updates 
 * `ms->exit_status` and returns `-1`.
 * 
 * @param ms The main shell structure, containing `heredoc_count` and 
 * `heredoc_files`.
 * @param limiter The delimiter string used to mark the end of the heredoc 
 * input.
 * @param token A pointer to the token containing heredoc-related information.
 * 
 * @return The file descriptor of the heredoc file on success, or `-1` if 
 * there was an error opening the file.
 */
int	handle_heredoc(t_ms *ms, char *limiter, t_token *token)
{
	int		temp_fd;
	int		fd;
	char	*filename;
	pid_t	pid;
	int		status;

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
		open_heredoc_file(filename, &temp_fd);
		read_input(temp_fd, limiter, token, ms);
		exit(0);
	}
	else
	{
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
	}
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		ms->exit_status = 1;
		return (-1);
	}
	return (fd);
}
