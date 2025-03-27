/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_blocks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 14:10:50 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 14:10:52 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Validates a sequence of tokens within a command block.
 * 
 * This function checks whether all tokens between `start` and `end` are valid. 
 * A valid token is considered to be a WORD, a redirection, or a DUMMY token. 
 * If an invalid token is found, the error flag is set, and the function 
 * returns an error.
 * 
 * @param start A pointer to the first token in the block.
 * @param end A pointer to the token marking the end of the block (not included 
 *            in validation).
 * @param err_flag A pointer to an integer flag used to indicate errors. If an 
 *                 invalid token is encountered, the flag is set to `1`.
 * 
 * @return `0` if the block is valid, `1` if an error is found.
 * 
 * @note If no tokens exist between `start` and `end`, the function still 
 *       returns `0`.
 */
int	check_block(t_token *start, t_token *end, int *err_flag) //check without
{ //what if the very first block starts from | symbol?
	t_token	*cur;
	//what if nothing in between
	cur = start;
	while (cur != end)
	{
		if (!(cur->type == WORD || is_redirect(cur->type)
				|| cur->type == DUMMY)) //dummy?
		{
			*err_flag = 1; //save place?
			return (1);
		}
		cur = cur->next;
	}
	return (0);
}

/**
 * @brief Creates a new command block from a sequence of tokens.
 * 
 * This function allocates and initializes a new `t_block` structure 
 * representing a block of tokens between `start` and `end`. If the block
 * validation fails, the function returns the original block list without
 * modification. If memory allocation fails, the error flag is set.
 * 
 * @param start A pointer to the first token of the block.
 * @param end A pointer to the last token of the block.
 * @param first_block A pointer to the head of the block list.
 * @param err_flag A pointer to an integer flag used to indicate errors.
 *                 If memory allocation fails, this flag is set to `1`.
 * 
 * @return A pointer to the updated list of blocks, with the new block appended 
 *         if successful. If an error occurs, the original block list is 
 *         returned.
 * 
 * @note If `first_block` is NULL, the new block becomes the first in the list.
 */
t_block	*create_block(t_token *start, t_token *end, t_block *first_block, int *err_flag)
{
	t_block	*new;
	t_block	*cur;

	if (check_block(start, end, err_flag))
		return (first_block);
	new = (t_block *)malloc(1 * sizeof(t_block));
	if (!new)
	{
		*err_flag = 1; //?
		return (first_block);
	}
	new->start = start;
	new->end = end;
	new->next = NULL;
	if (!first_block) //if blocks list is still empty
		return (new);
	cur = first_block;
	while (cur->next)
		cur = cur->next;
	cur->next = new; //put it as a last
	return (first_block);
}

/**
 * @brief Splits a sequence of tokens into command blocks.
 * 
 * This function iterates through the token list from `start` to `end` and 
 * creates a new command block whenever a PIPE token is encountered. Each 
 * block represents a segment of the command separated by pipes. The final 
 * block is created after the last PIPE or at the end of the sequence.
 * 
 * @param start A pointer to the first token in the sequence.
 * @param end A pointer to the last token in the sequence.
 * @param err_flag A pointer to an integer flag used to indicate errors. If 
 *                 an error occurs during block creation, the function 
 *                 returns `NULL` after freeing any allocated blocks.
 * 
 * @return A pointer to the first block in the created block list. If an 
 *         error occurs, returns `NULL`.
 * 
 * @note If the input contains no PIPE tokens, the function creates a 
 *       single block containing all tokens.
 */
t_block	*create_blocks_list(t_token *start, t_token *end, int *err_flag) //check r!
{
	t_token	*cur_token;
	t_block	*first_block;

	first_block = NULL;
	cur_token = start;
	while (cur_token->next && cur_token != end) //check r
	{
		if (cur_token->type == PIPE)
		{
			first_block = create_block(start, cur_token, first_block, err_flag);
			start = cur_token->next; //switch to the next one
		}
		cur_token = cur_token->next;
		//err_flag check
		if (*err_flag)
			return (clean_block_list(&first_block));
	}
	first_block = create_block(start, cur_token->next, first_block, err_flag);
	return (first_block);
}
