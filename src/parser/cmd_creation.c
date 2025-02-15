
#include "../../include/minishell.h"

static void	default_cmd_values(t_cmd *new, int num) //check r
{
	new->name = NULL;
	new->args = NULL;
	new->num = num;
	new->next = NULL;
	//some other fields
}

int	words_in_cmd_block(t_token *start, t_token *end)
{
	t_token	*cur;
	int		res;

	res = 0;
	cur = start;
	while (cur != end)
	{
		if (cur->type == WORD)
			res++;
		cur = cur->next;
	}
	return (res);
}

int	put_cmg_args(t_cmd *cmd, t_token *start, t_token *end)
{
	t_token	*cur;
	int		i;

	i = 0;
	cur = start;
	while (cur != end)
	{
		if (cur->type == WORD)
		{
			cmd->args[i] = ft_strdup(cur->data);
			if (!cmd->args[i])
				return (1);
			i++;
		}
		cur = cur->next;
	}
	cmd->args[i] = NULL;
	return (0);
}

t_cmd	*create_new_cmd(t_block *block, int num, t_ms *ms)
{
	t_cmd	*new;
	int		words;

	new = (t_cmd *)malloc(1 * sizeof(t_cmd));
	if (!new)
		return (NULL);
	if (ms) //delete
		default_cmd_values(new, num);
	words = words_in_cmd_block(block->start, block->end);
	new->args = (char **)malloc((words + 1) * sizeof(char *));
	if (!new->args)
		return (clean_cmd(new));
	if (put_cmg_args(new, block->start, block->end))
		return (clean_cmd(new));
	new->name = new->args[0];
	//redir?
	return (new);
}

t_cmd	*create_cmd_list(t_block *block, t_ms *ms)
{
	t_cmd	*first;
	t_cmd	*cur;
	int		i;

	if (!block)
		return (NULL); //is it needed?
	first = create_new_cmd(block, 0, ms);
	if (!first)
		return (NULL);
	block = block->next;
	cur = first;
	i = 1;
	while (block)
	{
		cur->next = create_new_cmd(block, i, ms);
		if (!cur->next)
			return (clean_cmd_list(&first));
		cur = cur->next;
		i++;
		block = block->next;
	}
	return (first);
}
