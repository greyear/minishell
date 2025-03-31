/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 13:45:51 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 13:45:54 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Validates a pipeline of commands separated by pipes (`|`):
 *        `<pipeline> ::= <command> { '|' <command> }`.
 * 
 * This function checks the validity of a pipeline consisting of multiple 
 * commands separated by pipes. It ensures that each command in the sequence 
 * is correctly formatted and that there are no syntax errors.
 * 
 * @param str A pointer to the input string to be validated.
 * @param err_flag A pointer to an integer flag used to indicate errors. 
 *                 If any validation fails, `err_flag` is set to `1`.
 * 
 * @return A pointer to the next token in the input string after processing 
 *         the pipeline. If an error is encountered, `err_flag` is set to `1` 
 *         and the function returns a pointer to the position where the 
 *         error occurred.
 */
char	*validate_pipeline(char *str, int *err_flag)
{
	char	*next;

	while (ft_isspace(*str))
		str++;
	next = validate_cmd(str, err_flag);
	if (next == str)
		*err_flag = 1;
	if (*err_flag == 1)
		return (next);
	if (*next == VERTICAL && *(next + 1) != VERTICAL)
	{
		next++;
		if (ft_isemptystring(next))
		{
			*err_flag = 1;
			return (next);
		}
		next = validate_pipeline(next, err_flag);
	}
	return (next);
}
