#include "../../include/minishell.h"

/**
 * @brief Appends a single character to a dynamically allocated string.
 * 
 * This function takes a character and appends it to the given string by creating 
 * a new string with the additional character. If memory allocation fails at any 
 * point, an error message is printed, and the shell's exit status is set to `MALLOC_ERR`.
 * 
 * @param result A pointer to the dynamically allocated string to which the character 
 *               will be appended. The original memory is freed after concatenation.
 * @param c The character to append.
 * @param ms A pointer to the `t_ms` structure, which manages shell-related data, 
 *           including exit status.
 * 
 * @return None. The function modifies `*result` and updates `ms->exit_status` on failure.
 */

static void	append_literal_char(char **result, char c, t_ms *ms)
{
	char	*substr;
	char	*temp;
	char	str[2];

	str[0] = c;
	str[1] = '\0';
	substr = ft_substr(str, 0, 1);
	if (!substr)
	{
		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
		return;
	}
	temp = ft_strjoin(*result, substr);
	if (!temp)
	{
		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
		free(substr);
		return;
	}
	free(substr);
	free(*result);
	*result = temp;
}

/**
 * @brief Extracts a key (variable name) from an export command argument.
 * 
 * This function scans the given argument string starting at index `*i` and extracts 
 * a valid key consisting of alphanumeric characters or underscores. If the key is `?`, 
 * it extracts only that character. The function dynamically allocates memory for the key 
 * and updates `*i` to reflect the new position in the string. If memory allocation fails, 
 * an error message is printed, and the shell's exit status is set to `MALLOC_ERR`.
 * 
 * @param args The string containing the export argument.
 * @param i A pointer to the current index in `args`, which will be updated after extraction.
 * @param ms A pointer to the `t_ms` structure, which manages shell-related data, including 
 *           exit status.
 * 
 * @return A newly allocated string containing the extracted key, or NULL on failure.
 */

static char	*extract_key_export(char *args, int *i, t_ms *ms)
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
	{
		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
		return (NULL);
	}
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
  if (ms->exit_status == MALLOC_ERR)
		return (1);
	return (0);
}

char	*handle_expansion(t_expand *exp, t_ms *ms)
{
	int		i;
	char	*result;
	//t_expand	*exp;

	result = ft_strdup("");
	if (!result)
	{
		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
		return (NULL);
	}
	i = 0;
	while (exp->data[i])
	{
    if (ms->exit_status == MALLOC_ERR)
			return (NULL);
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
	return (result);
}

