/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_creation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 14:11:05 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 14:11:07 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//mallocs checked

//mallocs checked

#include "../../include/minishell.h"

/**
 * @brief Initializes a command structure with default values.
 * 
 * This function sets up a newly allocated `t_cmd` structure with default 
 * values to ensure proper initialization before further processing. It 
 * assigns `NULL` to string fields, sets file descriptors to default values, 
 * and initializes the command index.
 * 
 * @param new A pointer to the `t_cmd` structure to be initialized.
 * @param num The command index, used for execution tracking.
 * 
 * @note This function does not allocate memory for `new`. It assumes `new` 
 *       is already allocated.
 */
static void	default_cmd_values(t_cmd *new, int num)
{
	new->name = NULL;
	new->args = NULL;
	new->num = num;
	new->infile = DEF;
	new->outfile = DEF;
	new->next = NULL;
}

/**
 * @brief Fills the command arguments array with WORD tokens.
 * 
 * This function iterates through tokens from `start` to `end`, copying the 
 * data of tokens of type `WORD` into the `args` array of the given `cmd` 
 * structure. The function ensures the last element of `args` is set to `NULL` 
 * for proper termination.
 * 
 * @param cmd A pointer to the command structure where arguments will be stored.
 * @param start A pointer to the first token in the block.
 * @param end A pointer to the token marking the end of the block (not included 
 *            in processing).
 * 
 * @return `0` on success, `1` if memory allocation for any argument fails.
 */
int	put_cmg_args(t_cmd *cmd, t_token *start, t_token *end, t_ms *ms)
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
			{
				print_malloc_set_status(ms);
				return (1);
			}
			i++;
		}
		cur = cur->next;
	}
	cmd->args[i] = NULL;
	return (0);
}

/**
 * @brief Processes redirections in a command block.
 * 
 * This function iterates through the tokens in the provided `block` and 
 * checks for redirection operators (e.g., `IN`, `OUT`, `APPEND`, `HEREDOC`). 
 * For each redirection operator, it calls the corresponding function 
 * (`put_infile_fd`, `put_outfile_fd`, or `put_heredoc_fd`) to handle the 
 * redirection by updating the file descriptors in the provided `cmd` structure. 
 * If a redirection fails (either input or output), the exit status of the 
 * shell is set to `1`, and the function exits early.
 * 
 * @param block A pointer to the block containing tokens that may represent 
 *              redirections.
 * @param cmd A pointer to the command structure where file descriptors for 
 *            redirections will be stored.
 * @param ms A pointer to the shell's state, used for updating the exit 
 *           status on failure.
 * 
 * @note The function handles input redirection (`IN`), output redirection 
 *       (`OUT`, `APPEND`), and heredoc (`HEREDOC`), and assumes that 
 *       `put_infile_fd`, `put_outfile_fd`, and `put_heredoc_fd` correctly 
 *       manage the file descriptor setup for each redirection type. If either 
 *       input or output file descriptors are not set or have errors (`NO_FD`), 
 *       the function will exit early with a failure status.
 */
void	redir_in_block(t_block *block, t_cmd *cmd, t_ms *ms)
{
	t_token	*cur;

	cur = block->start;
	while (cur != block->end)
	{
		if (is_redirect(cur->type) && g_sgnl != SIGINT)
		{
			if (cur->type == IN)
				put_infile_fd(cur, cmd);
			else if (cur->type == OUT || cur->type == APPEND)
				put_outfile_fd(cur, cmd);
			else if (cur->type == HEREDOC)
				put_heredoc_fd(cur, cmd, ms);
			if (cmd->infile == NO_FD || cmd->outfile == NO_FD)
			{
				ms->exit_status = 1;
				return ;
			}
		}
		cur = cur->next;
	}
}

/**
 * @brief Creates a new command structure from a given block of tokens.
 * 
 * This function allocates and initializes a new `t_cmd` structure, extracts 
 * arguments from the tokens within the specified block, and processes any 
 * redirection operators (input, output, or heredoc). The arguments and 
 * redirection file descriptors are stored in the `t_cmd` structure, which 
 * is then returned. The `default_cmd_values` function is called to set 
 * initial values for the new command, and memory is cleaned up if allocation 
 * fails at any step.
 * 
 * @param block A pointer to the block containing tokens representing the 
 *              command.
 * @param num An integer representing the command number, used to initialize 
 *            the `t_cmd`.
 * @param ms A pointer to the shell's state, passed to handle redirections 
 *           and possibly update the shell's exit status on failure.
 * 
 * @return A pointer to the newly created `t_cmd` structure, or `NULL` if any 
 *         allocation or setup fails.
 * 
 * @note The function relies on `words_in_cmd_block` to count the number of 
 *       arguments in the block, and on `put_cmg_args` to assign those 
 *       arguments to the new command. If memory allocation for the command 
 *       or arguments fails, the function ensures that allocated memory is 
 *       freed before returning `NULL`.
 */
t_cmd	*create_new_cmd(t_block *block, int num, t_ms *ms)
{
	t_cmd	*new;
	int		words;

	new = (t_cmd *)malloc(1 * sizeof(t_cmd));
	if (!new)
		return (print_malloc_set_status(ms));
	default_cmd_values(new, num);
	words = words_in_cmd_block(block->start, block->end);
	new->args = (char **)malloc((words + 1) * sizeof(char *));
	if (!new->args)
	{
		print_malloc_set_status(ms);
		return (clean_cmd(new));
	}
	if (put_cmg_args(new, block->start, block->end, ms))
		return (clean_cmd(new));
	new->name = new->args[0];
	redir_in_block(block, new, ms);
	return (new);
}

/**
 * @brief Creates a linked list of commands from a block of tokens.
 * 
 * This function processes a sequence of blocks (representing commands) 
 * and converts each block into a `t_cmd` structure. The commands are linked 
 * together in a list, with the first command being returned. For each block, 
 * the function calls `create_new_cmd` to create a new `t_cmd`, and links it 
 * to the list of commands. If memory allocation fails for any command, the 
 * list is cleaned up and `NULL` is returned.
 * 
 * @param block A pointer to the first block in the sequence of blocks that 
 *              needs to be converted into commands.
 * @param ms A pointer to the shell's state, which is passed to handle 
 *           redirections and command execution.
 * 
 * @return A pointer to the first `t_cmd` in the created command list, or 
 *         `NULL` if an error occurs during command creation or memory 
 *         allocation.
 * 
 * @note If the first block is `NULL`, the function immediately returns 
 *       `NULL`. Additionally, if memory allocation for any command fails, 
 *       the function cleans up the previously allocated commands before 
 *       returning `NULL`.
 */
t_cmd	*create_cmd_list(t_block *block, t_ms *ms)
{
	t_cmd	*first;
	t_cmd	*cur;
	int		i;

	if (!block)
		return (NULL);
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
