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
int	check_block(t_token *start, t_token *end, int *err_flag)
{
	t_token	*cur;

	cur = start;
	while (cur != end)
	{
		if (!(cur->type == WORD || is_redirect(cur->type)
				|| cur->type == DUMMY))
		{
			*err_flag = 1;
			return (1);
		}
		cur = cur->next;
	}
	return (0);
}

/**
 * @brief Creates and appends a new block to the list of command blocks.
 * 
 * This function allocates memory for a new `t_block` structure, 
 * initializing its `start` and `end` pointers with the given tokens. 
 * If memory allocation fails, it sets the `ms->err` flag to `1`, 
 * prints an error message, and returns the original `first_block` list.
 * 
 * If `first_block` is `NULL`, the function returns the newly created block
 * as the first element of the list. Otherwise, it appends the new block to 
 * the end of the existing linked list.
 * 
 * @param ms Pointer to the main shell structure containing error status.
 * @param start Pointer to the first token of the new block.
 * @param end Pointer to the last token (not included) of the new block.
 * @param first_block Pointer to the first block in the linked list.
 * 
 * @return Pointer to the head of the block list, either the same as 
 *         `first_block` or a newly created block if the list was empty.
 */
t_block	*create_block(t_ms *ms, t_token *start, t_token *end, \
		t_block *first_block)
{
	t_block	*new;
	t_block	*cur;

	if (check_block(start, end, &(ms->err)))
		return (first_block);
	new = (t_block *)malloc(1 * sizeof(t_block));
	if (!new)
	{
		ms->err = 1;
		print_malloc_set_status(ms);
		return (first_block);
	}
	new->start = start;
	new->end = end;
	new->next = NULL;
	if (!first_block)
		return (new);
	cur = first_block;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
	return (first_block);
}

/**
 * @brief Splits tokens into command blocks based on pipe (`|`) tokens.
 * 
 * This function iterates through a list of tokens, dividing them into 
 * separate command blocks whenever a `PIPE` token is encountered. 
 * Each created block represents a segment of the command sequence.
 * 
 * - If a `PIPE` token is found, a new block is created from `start` to 
 *   the current token, and `start` is updated to the next token.
 * - If an error occurs (`ms->err` is set), the function cleans up any 
 *   allocated blocks and returns `NULL`.
 * - Finally, it creates a block for the last segment of tokens and returns 
 *   the head of the linked list of blocks.
 * 
 * @param ms Pointer to the main shell structure containing error status.
 * @param start Pointer to the first token in the sequence.
 * @param end Pointer to the last token (not included) in the sequence.
 * 
 * @return Pointer to the head of the list of created blocks, or `NULL` 
 *         if an error occurs.
 */
t_block	*create_blocks_list(t_ms *ms, t_token *start, t_token *end)
{
	t_token	*cur_token;
	t_block	*first_block;

	first_block = NULL;
	cur_token = start;
	while (cur_token->next && cur_token != end)
	{
		if (cur_token->type == PIPE)
		{
			first_block = create_block(ms, start, cur_token, first_block);
			start = cur_token->next;
		}
		cur_token = cur_token->next;
		if (ms->err)
			return (clean_block_list(&first_block));
	}
	first_block = create_block(ms, start, cur_token->next, first_block);
	return (first_block);
}

/**
 * @brief Counts the number of WORD tokens in a command block.
 * 
 * This function iterates through a sequence of tokens from `start` to `end`, 
 * counting the number of tokens of type `WORD`. It helps determine the 
 * number of arguments present in a command block.
 * 
 * @param start A pointer to the first token in the block.
 * @param end A pointer to the token marking the end of the block (not included 
 *            in counting).
 * 
 * @return The number of WORD tokens found in the given range.
 * 
 * @note If `start` and `end` are the same, the function returns `0`.
 */
int	words_in_cmd_block(t_token *start, t_token *end)
{
	t_token	*cur;
	int		res;

	res = 0;
	cur = start;
	while (cur != end)
	{
		if (cur->type == WORD)
		{
			res++;
			if (cur->expanded == true && has_multiple_words(cur->data))
				res++;
		}
		cur = cur->next;
	}
	return (res);
}
