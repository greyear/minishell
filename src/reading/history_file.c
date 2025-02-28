
#include "../../include/minishell.h"

//fill_history
static void	read_from_history_file(int fd, t_ms *ms)
{
	int		i;
	char	*buf;

	i = 0;
	while (i < HISTORY_SIZE)
	{
		buf = get_next_line(fd);
		if (!buf)
			return ;
		add_history(buf);
		ms->history[i] = ft_strdup(buf);
		if (!ms->history[i])
		{
			free(buf); //do I need to clean all previously saved?
			return ;
		}
		free(buf);
		i++;
	}
}

//fetch_history_file
t_bool	open_create_history_file(t_ms *ms)
{
	int	fd;

	fd = open(HISTORY_FILE, O_RDONLY);
	if (fd < 0)
	{
		fd = open(HISTORY_FILE, O_CREAT, 0644);
		close(fd);
		return (SUCCESS);
	}
	read_from_history_file(fd, ms);
	close(fd);
	return (SUCCESS);
}

//add_to_history_file
void	write_to_history_file(t_ms *ms)
{
	int	fd;
	int	i;

	fd = open(HISTORY_FILE, O_TRUNC | O_RDWR);
	if (fd < 0)
		return ; //?
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
