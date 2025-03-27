/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cleaning.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 14:10:57 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 14:11:00 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Frees memory allocated for a command structure.
 * 
 * This function releases all dynamically allocated fields within a `t_cmd` 
 * structure, including the argument array. Finally, it frees the `t_cmd` 
 * structure itself.
 * 
 * @param cmd A pointer to the command structure to be freed.
 * 
 * @return Always returns `NULL` to ensure the pointer is reset after freeing.
 * 
 * @note If `cmd` is `NULL`, the function does nothing and simply returns `NULL`.
 */
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

/**
 * @brief Frees a linked list of command structures.
 * 
 * This function iterates through a linked list of `t_cmd` structures, freeing 
 * each command and its associated memory. Once all commands are freed, the 
 * pointer to the first command is set to `NULL`.
 * 
 * @param first A double pointer to the first command in the list. After
 *              execution, `*first` is set to `NULL` to prevent dangling
 *              pointers.
 * 
 * @return Always returns `NULL` to ensure proper pointer resetting.
 * 
 * @note If `first` or `*first` is `NULL`, the function does nothing.
 */
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

/**
 * @brief Frees a single command block structure.
 * 
 * This function deallocates memory associated with a `t_block` structure. 
 * If the block contains dynamically allocated fields, they should be freed 
 * before deallocating the structure itself.
 * 
 * @param block A pointer to the block to be freed.
 * 
 * @return Always returns `NULL` to ensure proper pointer resetting.
 * 
 * @note If `block` is `NULL`, the function does nothing.
 */
t_block	*clean_block(t_block *block)
{
	if (!block)
		return (NULL);
	//add fields
	free(block);
	return (NULL);
}

/**
 * @brief Frees a linked list of command blocks.
 * 
 * This function iterates through a linked list of `t_block` structures, freeing 
 * each block and its associated resources. It ensures that all allocated memory 
 * is properly freed.
 * 
 * @param first A pointer to the head of the list. The pointer is set to `NULL` 
 *              after cleanup.
 * 
 * @return Always returns `NULL` to facilitate pointer resetting.
 * 
 * @note If `first` is `NULL` or the list is empty, the function does nothing.
 */
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
