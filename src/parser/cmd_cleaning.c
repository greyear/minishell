
#include "../../include/minishell.h"

t_cmd	*clean_cmd(t_cmd *cmd)
{
	if (!cmd)
		return (NULL);
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

t_block	*clean_block(t_block *block)
{
	if (!block)
		return (NULL);
	//add fields
	free(block);
	return (NULL);
}

t_block	*clean_block_list(t_block **first)
{
	t_block	*cur;
	t_block	*next;

	if (!first || !*first)
		return (NULL);
	cur = *first;
	while (cur)
	{
		next = cur->next;
		clean_block(cur);
		cur = next;
	}
	*first = NULL;
	return (NULL);
}
