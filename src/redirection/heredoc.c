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

 static void	read_heredoc_input(int temp_fd, char *limiter, t_token *token, t_ms *ms)
 {
	 char	*line;
	 char	*expanded;

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
		 if (token->quote != SG_QUOT && token->quote != DB_QUOT)
		 {
			expanded = handle_expansion(line, ms);
			free(line);
			line = expanded;
		 }
		 write(temp_fd, line, ft_strlen(line));
		 free(line);
	 }
	 close(temp_fd);
 }

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