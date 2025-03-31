/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 13:45:34 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 13:45:38 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Validates a command, which can be a simple command or a pipeline 
 *        with an optional redirect.
 * 
 *        `<command> ::= <simple_cmd> | '(' <pipeline> ')' {<redirect>}`.
 * 
 * @param str A pointer to the null-terminated input string to be validated.
 * @param err_flag A pointer to an integer flag used to indicate errors. 
 *                 If an error is detected, the flag is set to `1`.
 * 
 * @return A pointer to the next token in the input string after processing 
 *         the command. If an error is encountered, `err_flag` is set to `1` 
 *         and the function returns a pointer to the position where the error 
 *         occurred.
 */
char	*validate_cmd(char *str, int *err_flag);

/**
 * @brief Validates and processes a pipeline followed by a redirection.
 * 
 * This function advances the input string pointer past a pipeline character 
 * (`|`) and ensures that the syntax is correct. It checks for empty input 
 * after the pipeline, validates the pipeline sequence, and ensures a closing 
 * parenthesis (`)`) is present before proceeding to validate the redirection 
 * syntax.
 * 
 * @param str The input string to be processed, expected to start after
 *            a pipeline.
 * @param err_flag A pointer to an error flag, which is set to `1` if a syntax 
 *                 error is found.
 * 
 * @return A pointer to the next valid position in the string after processing, 
 *         or the erroneous position if a syntax error occurs.
 */
static char	*pipeline_redirect(char *str, int *err_flag)
{
	char	*next;

	str++;
	if (ft_isemptystring(str))
	{
		*err_flag = 1;
		return (str);
	}
	next = validate_pipeline(str, err_flag);
	if (*err_flag == 1)
		return (next);
	if (*next != R_PARENT)
	{
		*err_flag = 1;
		return (next);
	}
	next++;
	while (ft_isspace(*next))
		next++;
	next = validate_redirect(next, err_flag);
	return (next);
}

char	*validate_cmd(char *str, int *err_flag)
{
	char	*next;

	next = validate_simple_cmd(str, err_flag);
	if (next != str || *err_flag == 1)
		return (next);
	else if (*str == L_PARENT)
		next = pipeline_redirect(str, err_flag);
	return (next);
}
