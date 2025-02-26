
#include "../../include/minishell.h"

/**
 * @brief Skips special tokens in the input string based on their type.
 * 
 * This function advances the index in the input string (`str`) to skip over 
 * special tokens such as redirections (`<<`, `>>`) and other non-word, non-space 
 * characters. It ensures correct parsing by handling multi-character tokens properly.
 * 
 * @param str A pointer to the input string being processed.
 * @param i A pointer to the current index in the string, which is updated as needed.
 * @param type The type of token being processed.
 */
void	skip_special_tokens(char *str, size_t *i, t_token_type type)
{
	if (!str || !i)
		return;
	if (str[*i + 1] && (type == HEREDOC || type == APPEND))
		(*i) += 2;
	else if (str[*i] && (type != WORD && type != SPACE))
		(*i)++;
}

/**
 * @brief Skips whitespace characters in a string.
 * 
 * This function advances the given index (`i`) in the string (`str`) 
 * past any leading whitespace characters. It ensures that parsing starts 
 * at the first non-whitespace character.
 * 
 * @param str A pointer to the input string.
 * @param i A pointer to the current index in the string, which is updated 
 *          to the next non-whitespace character.
 */
void	skip_whitespaces(char *str, size_t *i)
{
	if (!str || !i)
		return;
	while (str[*i] && ft_isspace(str[*i]))
		(*i)++;
}
