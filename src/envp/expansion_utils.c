#include "../../include/minishell.h"

/**
 * @brief Appends a new string segment to the result string.
 *
 * This function concatenates `new_part` to `*result`, updating the pointer
 * to the newly allocated string. It ensures proper memory management by
 * freeing the previous `*result` and `new_part` after joining.
 *
 * @param result Pointer to the existing result string. It will be updated
 *        to point to the new concatenated string.
 * @param new_part The string segment to be appended. It will be freed after use.
 *
 * @note If memory allocation for the new string fails, `new_part` is freed,
 *       but `*result` remains unchanged.
 */

static void	append_to_result(char **result, char *new_part)
{
	char	*temp;

	if (!new_part)
		return;
	temp = ft_strjoin(*result, new_part);
	if (!temp)
	{
		free(new_part);
		return;
	}
	free(*result);
	*result = temp;
	free(new_part);
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
 
static char	*find_env_value(char **envp, char *key, int len)
{
	int     i;
 
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, len) == 0)
		{
			if (envp[i][len] && envp[i][len] == '=')
				return (ft_strdup(envp[i] + len + 1));
		}
		i++;
	}
	return (ft_strdup(""));
}

/**
 * @brief Expands an environment variable key and appends its value to the result.
 *
 * This function resolves an environment variable key to its corresponding value.
 * - If the key is `"?"`, it converts `ms->exit_status` to a string.
 * - If the key starts with a digit, it appends an empty string.
 * - Otherwise, it searches for the key in the environment variables.
 * 
 * The expanded value is appended to `result` using `append_to_result()`.
 *
 * @param ms Pointer to the main shell structure containing `exit_status` and `envp`.
 * @param key The environment variable key.
 * @param key_len The length of the key.
 * @param result Pointer to the string where the expanded value will be appended.
 *
 */

void    expand_variable(t_ms *ms, char *key, int key_len, char **result)
{
	char    *expanded;

	if (!key || !*key)
		return;
	if (key[0] == '?')
		expanded = ft_itoa(ms->exit_status);
	else if (ft_isdigit(key[0]))
		expanded = ft_strdup("");
	else
		expanded = find_env_value(ms->envp, key, key_len);
	append_to_result(result, expanded);
}

char	*remove_extra_spaces(char *str)
{
	char	*new;
	int		i;
	int		j;
	int		space;

	if (!str)
		return (NULL);
	int len = ft_strlen(str);
	new = ft_calloc(len + 1, sizeof(char));
	if (!new)
		return (NULL);
	i = 0;
	j = 0;
	space = 1; //met space
	while (str[i])
	{
		if (ft_isspace(str[i]))
		{
			if (!space)
			{
				new[j++] = ' ';
				space = 1;
			}
		}
		else
		{
			new[j++] = str[i];
			space = 0; //met non space, reset
		}
		i++;
	}
	if (j > 0 && new[j - 1] == ' ')
		j--;
	new[j] = '\0';
	return (new);
}
