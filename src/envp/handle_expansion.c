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
	key = extract_key_export(args, i, ms);
	if (!key)
		return (1);
	expand_variable(ms, key, ft_strlen(key), result);
	free(key);
	return (0);
}

/**
 * @brief Expands environment variables and special symbols within a string.
 * 
 * This function processes a string (`args`) and expands any environment variables 
 * or special symbols (such as `$` followed by a variable) in the string. It traverses 
 * the string, identifies the variables or special symbols, and appends the expanded 
 * values to a result string. If a part of the string is not a variable, it is added 
 * to the result string as a literal character.
 * 
 * @param args The input string that may contain environment variables or special symbols 
 *             (such as `$` for variables or `?` and `$` for special symbols).
 * @param ms A pointer to the `t_ms` structure, which contains the environment variables 
 *           and other necessary data.
 * 
 * @return A new string with environment variables and special symbols expanded. 
 *         If memory allocation fails, returns `NULL`. The caller is responsible 
 *         for freeing the returned string.
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
			&& !ft_isspace(args[i + 1]) && args[i + 1] != '/') //new slash to fix 303&307 parsing hell
		{
			if (handle_dollar_expansion(&result, args, &i, ms))
			{
				free(result);
				return (NULL);
			}
		}
		else
			append_literal_char(&result, args[i++], ms);
	}
	return (result);
}