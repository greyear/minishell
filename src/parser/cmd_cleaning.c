
#include "../../include/minishell.h"

t_cmd	*clean_cmd(t_cmd *cmd)
{
	if (!cmd)
		return (NULL);
	if (cmd->name)
		free(cmd->name);
	if (cmd->args)
		clean_arr(&(cmd->args));
	//add another fields
	free(cmd);
	return (NULL);
}

t_cmd	*clean_cmd_list(t_cmd **first)
{
	t_cmd	*cur;
	t_cmd	*next;

	if (!first || !*first)
		return (NULL);
	cur = *first;
	while (cur)
	{
		next = cur->next;
		clean_cmd(cur);
		cur = next;
	}
	*first = NULL;
	return (NULL);
}
