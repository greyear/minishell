
#include "../../include/minishell.h"

//zero_history
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

//add_to_history_array
static void	fill_ms_history_line(char *line, t_ms *ms) //fill in struct's field
{
	if (ms->history[ms->history_num])
		free(ms->history[ms->history_num]); //free this particular line
	ms->history[ms->history_num] = ft_strdup(line); //copy given line into it
	if (!ms->history[ms->history_num])
		return ; //this one was unsuccessful but it's okay mb
	if (ms->history_num == HISTORY_SIZE - 1)
		ms->history_num = 0;
	else
		ms->history_num++;
}

//add_to_history
void	add_line_to_history(char *line, t_ms *ms)
{
	if (line[0] != '\0')
	{
		add_history(line);
		//check
		fill_ms_history_line(line, ms);
	}
}

//free_history
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
