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

static int	handle_dollar_expansion(char **result, t_expand *exp, int *i, t_ms *ms)

{
	//char	*key;

	(*i)++;
	exp->key = extract_key_export(exp->data, i);
	if (!exp->key)
		return (1);
	exp->len = ft_strlen(exp->key);
	expand_variable(ms, exp, result);
	free(exp->key);
	return (0);
}

char	*handle_expansion(t_expand *exp, t_ms *ms)
{
	int		i;
	char	*result;
	//t_expand	*exp;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (exp->data[i])
	{
		if (exp->data[i] == '$' && exp->data[i + 1] && exp->data[i + 1] != '$'
			&& !ft_isspace(exp->data[i + 1]) && exp->data[i + 1] != '/') //new slash to fix 303&307 parsing hell
		{
			if (handle_dollar_expansion(&result, exp, &i, ms))
			{
				free(result);
				return (NULL);
			}
		}
		else
			append_literal_char(&result, exp->data[i++]);
	}
	//printf("res of exp: %s\n", result);
	return (result);
}

