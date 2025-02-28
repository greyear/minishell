#include "../../include/minishell.h"

void cleanup_heredocs(char **filenames)
{
    int	i;
	
	i = 0;
    while (filenames[i])
	{
        if (unlink(filenames[i]) == -1)  // Remove file
		{
			perror("unlink fail\n");
			return;
		}
		free(filenames[i]);    // Free the memory for the filename
        i++;
    }
    free(filenames);  // Free the array of filenames
}

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

static void	read_heredoc_input(int temp_fd, char *limiter)
{
	char *line;

	while (1)
	{
		write(STDOUT_FILENO, "heredoc> ", 9);
		line = get_next_line(STDIN_FILENO);
		if (!line)
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