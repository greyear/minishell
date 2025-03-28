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
 * @brief Creates a new command block from a sequence of tokens.
 * 
 * This function initializes a new `t_block` structure representing a command 
 * block. The block starts from `ms->tokens` and ends at `end`. If memory 
 * allocation fails, the function sets an error flag, prints an error message, 
 * and returns the original `first_block` without modification.
 * 
 * If `first_block` is `NULL`, the new block is returned as the first block in 
 * the list. Otherwise, it is appended to the end of the existing block list.
 * 
 * @param ms A pointer to the main shell structure containing tokenized input.
 * @param end A pointer to the token marking the end of the block.
 * @param first_block A pointer to the first block in the linked list of blocks.
 * @param err_flag A pointer to an integer flag used to indicate errors. If 
 *                 memory allocation fails, the flag is set to `1`.
 * 
 * @return A pointer to the first block in the linked list (either updated or 
 *         unchanged).
 * @note If `first_block` is `NULL`, a new block is created and returned as 
 *       the first block.
 * @note If memory allocation fails, the function does not modify the existing 
 *       block list.
 */
/*
RIGHT VERSION
t_block	*create_block(t_ms *ms, t_token *end, t_block *first_block, int *err)
{
	t_block	*new;
	t_block	*cur;

	if (check_block(ms->tokens, end, err))
		return (first_block);
	new = (t_block *)malloc(1 * sizeof(t_block));
	if (!new)
	{
		*err = 1;
		print_malloc_set_status(ms);
		return (first_block);
	}
	new->start = ms->tokens;
	new->end = end;
	new->next = NULL;
	if (!first_block)
		return (new);
	cur = first_block;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
	return (first_block);
}*/

t_block	*create_block(t_ms *ms, t_token *start, t_token *end, t_block *first_block, int *err)
{
	t_block	*new;
	t_block	*cur;

	if (check_block(start, end, err))
		return (first_block);
	new = (t_block *)malloc(1 * sizeof(t_block));
	if (!new)
	{
		*err = 1;
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
 * @brief Creates a linked list of command blocks by splitting tokens at 
 * pipes.
 * 
 * This function processes a sequence of tokens and splits them into command 
 * blocks whenever a `PIPE` token is encountered. Each block is created using 
 * `create_block()`, and all blocks are linked together in a list. The function 
 * stops processing if an error occurs (e.g., memory allocation failure).
 * 
 * @param ms A pointer to the main shell structure containing tokenized input.
 * @param end A pointer to the token marking the end of parsing.
 * @param err_flag A pointer to an integer flag used to indicate errors. If an 
 *                 error occurs during block creation, the flag is set to `1`, 
 *                 and all allocated blocks are cleaned up before returning 
 *                 `NULL`.
 * 
 * @return A pointer to the first block in the linked list, or `NULL` if an 
 *         error occurs.
 * 
 * @note If a `PIPE` token is found, a new block is created before the pipe, and 
 *       parsing continues with the next token.
 * @note If an error occurs, the function calls `clean_block_list()` to free 
 *       allocated memory.
 */
/* RIGHT VERSION
t_block	*create_blocks_list(t_ms *ms, t_token *end, int *err_flag)
{
	t_token	*cur_token;
	t_block	*first_block;

	first_block = NULL;
	cur_token = ms->tokens;
	while (cur_token->next && cur_token != end)
	{
		if (cur_token->type == PIPE)
		{
			first_block = create_block(ms, cur_token, first_block, err_flag);
			ms->tokens = cur_token->next;
		}
		cur_token = cur_token->next;
		if (*err_flag)
			return (clean_block_list(&first_block));
	}
	first_block = create_block(ms, cur_token->next, first_block, err_flag);
	return (first_block);
}*/

t_block	*create_blocks_list(t_ms *ms, t_token *start, t_token *end, int *err_flag)
{
	t_token	*cur_token;
	t_block	*first_block;

	first_block = NULL;
	cur_token = start;
	while (cur_token->next && cur_token != end)
	{
		if (cur_token->type == PIPE)
		{
			first_block = create_block(ms, start, cur_token, first_block, err_flag);
			start = cur_token->next;
		}
		cur_token = cur_token->next;
		if (*err_flag)
			return (clean_block_list(&first_block));
	}
	first_block = create_block(ms, start, cur_token->next, first_block, err_flag);
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
			res++;
		cur = cur->next;
	}
	return (res);
}
