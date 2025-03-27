/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 13:45:59 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 13:46:02 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//mallocs checked

#include "../../include/minishell.h"

/**
 * @brief Validates a redirection in the input string:
 *        `<redirect> ::= ( '<' | '>' | '<<' | '>>' ) <word>`.
 * 
 * A valid redirection must start with either a single `<` or `>` character, 
 * or double `<<` or `>>` characters, followed by a valid word (usually a 
 * file name). The function handles multiple redirections and ensures that 
 * the redirection syntax is correct. It also checks for unclosed quotes 
 * and other potential errors.
 * 
 * @param str A pointer to the input string to be validated.
 * @param err_flag A pointer to an integer flag used to indicate errors. 
 *                 If any validation fails, `err_flag` is set to `1`.
 * 
 * @return A pointer to the next token in the input string after processing 
 *         the redirection. If an error is encountered, `err_flag` is set to 
 *         `1` and the function returns a pointer to the position where the 
 *         error occurred.
 */
char	*validate_redirect(char *str, int *err_flag)
{
	char	*next;

	if (str[0] != '<' && str[0] != '>')
		return (str);
	else if (ft_strncmp(str, "<<", 2) == 0 || \
			ft_strncmp(str, ">>", 2) == 0)
		str += 2;
	else if (ft_strncmp(str, "<", 1) == 0 || \
			ft_strncmp(str, ">", 1) == 0)
		str += 1;
	while (ft_isspace(*str))
		str++;
	next = validate_word(str, err_flag);
	if (next == str)
		*err_flag = 1;
	if (*err_flag == 1 || *next == R_PARENT)
		return (next);
	while (ft_isspace(*next))
		next++;
	next = validate_redirect(next, err_flag);
	return (next);
}
