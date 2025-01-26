
#include "../../include/minishell.h"

/**
 * @brief Validates a word in the input string, handling both regular words and quoted words:
 *        <word> | "any" | 'any'.
 * A word is a sequence of characters that does not contain special symbols. Special symbols 
 * are not part of a word and are treated as separate tokens. The function also handles words
 * enclosed in single or double quotes, which can contain special characters but are still treated 
 * as valid word content.
 * The function works as follows:
 * - It returns the null-terminator if the word is a simple, unquoted word.
 * - If the word is enclosed in quotes (either single or double), the function returns the first character after the closing quote.
 * - If the word has an unclosed quote, the function sets an error flag and returns the null-terminator.
 * - If a special symbol is encountered, it returns the special symbol.
 * 
 * @param str A pointer to the input string representing the word to be validated.
 * @param err_flag A pointer to an integer flag used to indicate errors. If an unclosed quote is found, 
 *                 the flag is set to `1`.
 * 
 * @return A pointer to the next character after the word. If an error occurs (e.g., unclosed quote), the 
 *         function sets `err_flag` to `1` and returns the null-terminator.
 */
char	*validate_word(char *str, int *err_flag);

static int	ft_special(int c)
{
	if (c == '|' || c == '&' || c == '<' || c == '>' || \
				c == '(' || c == ')' || c == ' '|| \
				c == '\'' || c == '\"')
		return (1);
	return (0);
}

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
	int	len_inside;

	while (*str && !ft_special(*str))
		str++;
	if (*str == '\'' || *str == '\"')
		str += length_inside_quotes(str, err_flag);
	return (str);
}
