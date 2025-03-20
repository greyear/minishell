#include "../../include/minishell.h"

/**
 * @brief Appends a single character to a dynamically allocated string.
 * 
 * This function takes a character and appends it to the given string by 
 * creating a new string with the additional character. If memory allocation 
 * fails at any point, an error message is printed, and the shell's exit 
 * status is set to `MALLOC_ERR`.
 * 
 * @param result A pointer to the dynamically allocated string to which the 
 *               character will be appended. The original memory is freed 
 *               after concatenation.
 * @param c The character to append.
 * @param ms A pointer to the `t_ms` structure, which manages shell-related 
 *           data, including exit status.
 * 
 * @return None. The function modifies `*result` and updates `ms->exit_status` 
 *         on failure.
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
		return ;
	}
	temp = ft_strjoin(*result, substr);
	if (!temp)
	{
		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
		free(substr);
		return ;
	}
	free(substr);
	free(*result);
	*result = temp;
}

/**
 * @brief Extracts a key (variable name) from an export command argument.
 * 
 * This function scans the given argument string starting at index `*i` and 
 * extracts a valid key consisting of alphanumeric characters or underscores. 
 * If the key is `?`, it extracts only that character. The function dynamically 
 * allocates memory for the key and updates `*i` to reflect the new position in 
 * the string. If memory allocation fails, an error message is printed, and the 
 * shell's exit status is set to `MALLOC_ERR`.
 * 
 * @param args The string containing the export argument.
 * @param i A pointer to the current index in `args`, which will be updated 
 *          after extraction.
 * @param ms A pointer to the `t_ms` structure, which manages shell-related 
 *           data, including exit status.
 * 
 * @return A newly allocated string containing the extracted key, or NULL 
 *         on failure.
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

/**
 * @brief Handles the expansion of a variable prefixed with `$` in a string.
 * 
 * This function processes a dollar-sign-prefixed (`$`) variable in a string by 
 * extracting its key, determining its length, and expanding it to its 
 * corresponding value. The expanded value is then appended to the result string.
 * The function also manages memory allocation failures and updates the shell's 
 * exit status if needed.
 * 
 * @param result A pointer to the string where the expanded value will be 
 *               appended.
 * @param exp A pointer to the `t_expand` structure, which stores expansion-
 *            related data.
 * @param i A pointer to the current index in `exp->data`, which is incremented 
 *          accordingly.
 * @param ms A pointer to the `t_ms` structure, which contains shell-related 
 *           information.
 * 
 * @return Returns `1` if memory allocation fails, setting `ms->exit_status` 
 *         to `MALLOC_ERR`, otherwise returns `0` on success.
 */

static int	dollar_expansion(char **result, t_expand *exp, int *i, t_ms *ms)
{
	(*i)++;
	exp->key = extract_key_export(exp->data, i, ms);
	if (!exp->key)
		return (1);
	exp->len = ft_strlen(exp->key);
	expand_variable(ms, exp, result);
	free(exp->key);
	if (ms->exit_status == MALLOC_ERR)
		return (1);
	return (0);
}

/**
 * @brief Expands variables in a string and appends them to the result.
 *
 * This function iterates through the `exp->data` string, looking for dollar 
 * signs ('$') that indicate the presence of variables to expand. When a
 * variable is detected, it calls `dollar_expansion` to handle the expansion 
 * and append the result. If memory allocation fails at any point, the 
 * function cleans up and returns `NULL`. Literal characters are appended 
 * directly to the `result` string. The function returns the final expanded
 *  string.
 *
 * @param exp The structure containing the string data to be expanded and other 
 *            expansion details.
 * @param ms The shell structure containing execution state information, 
 *           including exit status.
 * @param result A pointer to the string where expanded variable values and 
 *               literals will be appended.
 * 
 * @return The expanded result string or `NULL` if an error occurs during memory 
 *         allocation.
 */

static char	*expand(t_expand *exp, t_ms *ms, char **result)
{
	int		i;

	i = 0;
	while (exp->data[i])
	{
		if (exp->data[i] == '$' && exp->data[i + 1] && exp->data[i + 1] != '$'
			&& !ft_isspace(exp->data[i + 1]) && exp->data[i + 1] != '/')
		{
			if (dollar_expansion(result, exp, &i, ms))
			{
				free(*result);
				return (NULL);
			}
		}
		else
			append_literal_char(result, exp->data[i++], ms);
		if (ms->exit_status == MALLOC_ERR)
		{
			free(*result);
			return (NULL);
		}
	}
	return (*result);
}

/**
 * @brief Handles the expansion of variables in a string.
 *
 * This function initializes an empty string, attempts to allocate memory for 
 * it, and calls the `expand` function to process the variable expansion. If 
 * memory allocation fails, it prints an error message, sets the exit status 
 * to `MALLOC_ERR`, and returns `NULL`. Otherwise, it returns the result of 
 * the expansion.
 *
 * @param exp The structure containing the string data to be expanded and other 
 *            expansion details.
 * @param ms The shell structure containing execution state information, 
 *           including exit status.
 * 
 * @return A string containing the expanded variables, or `NULL` if memory 
 *         allocation fails.
 */

char	*handle_expansion(t_expand *exp, t_ms *ms)
{
	char	*result;

	result = ft_strdup("");
	if (!result)
	{
		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
		return (NULL);
	}
	return (expand(exp, ms, &result));
}
