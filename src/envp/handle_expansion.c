#include "../../include/minishell.h"

/**
 * @brief Appends a single character to an existing string.
 * 
 * This function takes a character (`c`) and appends it to the existing string (`result`). 
 * The character is first converted to a string and then joined with the existing string.
 * After the operation, the memory for the temporary substring is freed, and the `result` 
 * string is updated to include the appended character.
 * 
 * @param result A pointer to the string that will receive the appended character. 
 *               It will be updated with the new string.
 * @param c The character to append to the string.
 */

static void	append_literal_char(char **result, char c)
{
	char	*substr;
	char	*temp;
	char	str[2];

	str[0] = c;
	str[1] = '\0';
	substr = ft_substr(str, 0, 1);
	if (!substr)
		return;
	temp = ft_strjoin(*result, substr);
	if (!temp)
	{
		free(substr);
		return;
	}
	free(substr);
	free(*result);
	*result = temp;
}

 /**
  * @brief Extracts a key from a string (e.g., for export purposes).
  * 
  * This function extracts a valid key (starting with an alphanumeric character or underscore, and potentially 
  * including alphanumeric characters or underscores) from the provided string, starting at the position 
  * specified by the `i` index. It ensures that the key does not start with an invalid character (like a digit, 
  * `?`, `$`, or a space). The extracted key is returned as a new string, and the `i` index is updated to point 
  * to the next position in the original string.
  * 
  * @param args The string from which the key is to be extracted.
  * @param i A pointer to the current index in the string, which is updated after the key is extracted.
  * 
  * @return A newly allocated string containing the extracted key. If no key is found, an empty string is returned.
  */
 
static char	*extract_key_export(char *args, int *i)
{
	int		x;
	char	*key;
  
	x = 0;
	if (args[*i] == '?')
		x = 1;
	else
	{
		while (args[*i + x] && (ft_isalnum(args[*i + x])
			|| args[*i + x] == '_'))
			x++;
	}
	key = ft_substr(args, *i, x);
	if (!key)
		return (NULL);
	*i += x;
	return (key);
}

/**
 * @brief Processes a variable expansion starting with `$` and appends its value to the result.
 *
 * This function extracts the variable key following a `$`, resolves its value (e.g., from environment 
 * variables or special cases), and appends the expanded value to the result string. After the expansion, 
 * it updates the index to point to the character after the expanded variable.
 * 
 * The key is extracted using `extract_key_export()` and the value is expanded with `expand_variable()`.
 *
 * @param result A pointer to the string where the expanded value will be appended.
 * @param args The original string containing the variable to expand.
 * @param i A pointer to the current index in the `args` string. It is updated after processing the variable.
 * @param ms A pointer to the main shell structure containing `exit_status` and `envp`.
 *
 * @return 0 if the expansion was successful, or 1 if an error occurred (e.g., memory allocation failure).
 * 
 */


static int	handle_dollar_expansion(char **result, char *args, int *i, t_ms *ms)
{
	char	*key;

	(*i)++;
	key = extract_key_export(args, i);
	if (!key)
		return (1);
	expand_variable(ms, key, ft_strlen(key), result);
	free(key);
	return (0);
}

/**
 * @brief Expands variables in a string by resolving `$` references and appending 
 *        their values to the result.
 *
 * This function processes a string, looking for variables marked by the `$` symbol, 
 * and expands them by looking up their values. The process continues until all 
 * variables are resolved or the string is fully traversed.
 * 
 * If a `$` is followed by a valid key, its value is expanded using `handle_dollar_expansion()`. 
 * If the `$` is followed by anything else (including a space or invalid character), the function
 * appends the literal character to the result.
 *
 * The function returns the final expanded string.
 *
 * @param args The string to expand, which may contain variable references starting with `$`.
 * @param ms Pointer to the main shell structure containing `exit_status` and `envp`.
 *
 * @return A newly allocated string containing the expanded variables. 
 *         If an error occurs during expansion, `NULL` is returned.
 */


char	*handle_expansion(char *args, t_ms *ms)
{
	int		i;
	char	*result;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (args[i])
	{
		if (args[i] == '$' && args[i + 1] && args[i + 1] != '$'
			&& !ft_isspace(args[i + 1]))
		{
			if (handle_dollar_expansion(&result, args, &i, ms))
			{
				free (result);
				return (NULL);
			}
		}
		else
			append_literal_char(&result, args[i++]);
	}
	return (result);
}