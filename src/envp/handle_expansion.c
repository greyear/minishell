#include "../../include/minishell.h"

/**
 * @brief Verifies if a given key is valid based on its first character.
 * 
 * This function checks if the provided key starts with a digit. If it does, the key is considered
 * invalid and the function returns `NULL`. If the key is valid, it returns a duplicate of the key.
 * 
 * @param key The key string to be verified.
 * 
 * @return A duplicate of the key if it is valid (does not start with a digit), or `NULL` if it is invalid.
 */

static char	*verify_key(char *key)
{
	if (ft_isdigit(key[0]))
		return (NULL);
	return (ft_strdup(key));
}

static char	*handle_special_keys(char *key, t_ms *ms)
{
    if (key[0] == '?')
        return ft_itoa(ms->exit_status);
    if (key[0] == '$')
        return ft_itoa(getpid());
    return (NULL);
}

/**
 * @brief Searches for the value of a given environment variable in the environment.
 * 
 * This function looks through the `envp` array to find a string that starts with the specified key.
 * If a match is found, it returns the value associated with the environment variable, which is 
 * the part of the string after the `=` character. If no match is found, it returns an empty string.
 * 
 * @param envp The array of environment variables, where each element is a string in the format "KEY=VALUE".
 * @param key The key of the environment variable whose value is to be retrieved.
 * @param len The length of the key to match.
 * 
 * @return A string containing the value associated with the environment variable, or an empty string if not found.
 */

char	*find_env_value(char **envp, char *key, int len)
{
    int	i;

	i = 0;
    while (envp[i])
    {
        if (ft_strncmp(envp[i], key, len) == 0)
        {
            if (envp[i][len] == '=')
                return (ft_strdup(envp[i] + len + 1));
        }
        i++;
    }
    return (ft_strdup(""));
}

/**
 * @brief Expands a given key into its corresponding environment value or special value.
 * 
 * This function first checks if the key corresponds to any special variables (e.g., `$?` for exit status, 
 * `$$` for process ID) and returns the appropriate value if found. If the key doesn't match any special variables,
 * it verifies the key and searches for its value in the environment variables. If the key is invalid, it returns 
 * an empty string.
 * 
 * @param envp The array of environment variables (e.g., `environ`).
 * @param key The key to be expanded, which could be an environment variable or a special key.
 * @param len The length of the key to match in the environment variables.
 * @param ms A pointer to the main shell structure, used to access the exit status for special keys.
 * 
 * @return A string containing the expanded value for the key. If the key is invalid, an empty string is returned.
 */

char	*expand_key(char **envp, char *key, int len, t_ms *ms)
{
    char	*check;
    char	*res;

    res = handle_special_keys(key, ms);
    if (res)
        return (res);
    check = verify_key(key);
    if (!check)
        return (ft_strdup(""));
    key = check;
    res = find_env_value(envp, key, len);
    free(key);
    return (res);
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

char	*extract_key_export(char *args, int *i)
{
	int		x;
	char	*key;

	x = 0;
	if (ft_isdigit(args[*i]) || args[*i] == '?'
		|| args[*i] == '$' || args[*i] == ' ')
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
 * @brief Expands a variable by looking up its value in the environment or handling special cases.
 * 
 * This function takes a key (which represents a variable) and attempts to expand it by looking up its value
 * in the environment. If the key represents a special variable (e.g., `$` or `?`), the function handles these 
 * cases accordingly. The key is freed after being used to retrieve the expanded value.
 * 
 * @param ms A pointer to the shell state, containing environment variables and other necessary data.
 * @param key The variable key to be expanded (e.g., `$HOME` or `$1`).
 * @param key_len The length of the key, used to find the correct value in the environment variables.
 * 
 * @return A newly allocated string containing the expanded value of the key. If the key is not found, an empty
 *         string is returned. The original `key` is freed.
 */

char	*expand_variable(t_ms *ms, char *key, int key_len)
{
	char	*expanded_value;

	expanded_value = expand_key(ms->envp, key, key_len, ms);
	free(key);
	return (expanded_value);
}

/**
 * @brief Appends a new part to an existing string, reallocating memory as needed.
 * 
 * This function takes an existing string (`result`) and appends a new string (`new_part`) to it.
 * The new string is joined with the existing string, and the memory for the previous `result`
 * is freed after the operation. The `new_part` string is also freed after it is appended.
 * 
 * @param result A pointer to the string that will receive the appended value. It will be updated 
 *               with the new combined string.
 * @param new_part The string to append to `result`. This string will be freed after the operation.
 */

void	append_to_result(char **result, char *new_part)
{
	char	*temp;

	temp = ft_strjoin(*result, new_part);
	if (!temp)
		return;
	free(*result);
	*result = temp;
	free(new_part);
}

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

void	append_literal_char(char **result, char c)
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
	char	*key;
	char	*expanded;
	char	*result;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (args[i])
	{
		if (args[i] == '$' && args[i + 1] && !ft_isspace(args[i + 1]))
		{
			i++;
			key = extract_key_export(args, &i);
			expanded = expand_variable(ms, key, ft_strlen(key));
			append_to_result(&result, expanded);
		}
		else
			append_literal_char(&result, args[i++]);
	}
	return (result);
}