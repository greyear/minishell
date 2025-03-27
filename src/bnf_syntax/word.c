/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 13:46:18 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 13:46:23 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//mallocs checked

#include "../../include/minishell.h"

/**
 * @brief Validates a word in the input string, handling both regular words 
 *        and quoted words: <word> | "any" | 'any'.
 * 
 * A word is a sequence of characters that does not contain special symbols. 
 * Special symbols are not part of a word and are treated as separate tokens. 
 * The function also handles words enclosed in single or double quotes, which 
 * can contain special characters but are still treated as valid word content.
 * The function works as follows:
 * - It returns the null-terminator if the word is a simple, unquoted word.
 * - If the word is enclosed in quotes (either single or double), the function 
 *   returns the first character after the closing quote.
 * - If the word has an unclosed quote, the function sets an error flag and 
 *   returns the null-terminator.
 * - If a special symbol is encountered, it returns the special symbol.
 * 
 * @param str A pointer to the input string representing the word to be 
 *            validated.
 * @param err_flag A pointer to an integer flag used to indicate errors. 
 *                 If an unclosed quote is found, the flag is set to `1`.
 * 
 * @return A pointer to the next character after the word. If an error occurs 
 *         (e.g., unclosed quote), the function sets `err_flag` to `1` and 
 *         returns the null-terminator.
 */
char	*validate_word(char *str, int *err_flag);

/**
 * @brief Checks if a character is a special shell-related character.
 * 
 * This function determines whether the given character `c` is considered 
 * special in the context of shell parsing. Special characters include 
 * pipes (`|`), ampersands (`&`), redirection symbols (`<`, `>`), spaces, 
 * and quotes (`'`, `"`). These characters typically serve as syntax elements 
 * rather than part of a regular word.
 * 
 * @param c The character to be checked.
 * 
 * @return Returns `1` if `c` is a special character, otherwise returns `0`.
 */
int	ft_special(int c)
{
	if (c == '|' || c == '&' || c == '<' || c == '>'
		|| c == ' ' || c == '\'' || c == '\"')
		return (1);
	return (0);
}

/**
 * @brief Calculates the length of a quoted substring.
 * 
 * This function determines the length of a substring enclosed within 
 * single or double quotes. It starts from the first character in `str`, 
 * treating it as the opening quote, and counts characters until it 
 * encounters a matching closing quote or reaches the end of the string. 
 * If no closing quote is found, the `err_flag` is set to `1`, indicating 
 * a syntax error.
 * 
 * @param str The input string, expected to start with a quote character.
 * @param err_flag A pointer to an error flag, which is set to `1` if no 
 *                 matching closing quote is found.
 * 
 * @return The number of characters inside the quotes, excluding the 
 *         opening and closing quotes. Returns an incomplete length if 
 *         no closing quote is found.
 */
static int	length_inside_quotes(char *str, int *err_flag)
{
	char	any_quote;
	int		len;

	any_quote = *str;
	len = 0;
	str++;
	while (*str != '\0' && *str != any_quote)
	{
		len++;
		str++;
	}
	if (*str != any_quote)
		*err_flag = 1;
	return (len);
}

char	*validate_word(char *str, int *err_flag)
{
	while (*str && !ft_special(*str))
		str++;
	if (*str == '\'' || *str == '\"')
	{
		str = str + length_inside_quotes(str, err_flag);
		if (*err_flag == 0)
			str = str + 2;
		else if (*err_flag == 1)
			str = str + 1;
	}
	return (str);
}
