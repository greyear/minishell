#include "../../include/minishell.h"

/**
 * @brief Appends a dynamically allocated string to the result string.
 * 
 * This function concatenates `new_part` to `*result`, creating a new dynamically 
 * allocated string. The original `*result` is freed after concatenation. If memory 
 * allocation fails, an error message is printed, the shell's exit status is set to 
 * `MALLOC_ERR`, and `new_part` is freed.
 * 
 * @param result A pointer to the dynamically allocated result string. It is updated 
 *               with the newly concatenated string.
 * @param new_part A dynamically allocated string to be appended to `*result`. It 
 *                 is freed after use.
 * @param ms A pointer to the `t_ms` structure, which manages shell-related data, 
 *           including exit status.
 * 
 * @return None. The function modifies `*result` and updates `ms->exit_status` on failure.
 */

static void	append_to_result(char **result, char *new_part, t_ms *ms)
{
	char	*temp;

	if (!new_part)
		return;
	temp = ft_strjoin(*result, new_part);
	if (!temp)
	{

		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
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
	int	i;
 
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
 * @brief Expands an environment variable and appends its value to the result string.
 * 
 * This function retrieves the value of the environment variable specified by `key` 
 * and appends it to `*result`. If the key is `"?"`, it expands to the shell's 
 * exit status. If the key starts with a digit, it expands to an empty string. 
 * If memory allocation fails during expansion or appending, an error message is 
 * printed, and the shell's exit status is set to `MALLOC_ERR`.
 * 
 * @param ms A pointer to the `t_ms` structure, which contains environment variables 
 *           and shell-related data, including the exit status.
 * @param key A string representing the environment variable name.
 * @param key_len The length of the `key` string.
 * @param result A pointer to the dynamically allocated result string. The expanded 
 *               value is appended to it.
 * 
 * @return None. The function modifies `*result` and updates `ms->exit_status` on failure.
 */

void	expand_variable(t_ms *ms, char *key, int key_len, char **result)
{
	char	*expanded;

	if (!key || !*key)
		return;
	if (key[0] == '?')
		expanded = ft_itoa(ms->exit_status);
	else if (ft_isdigit(key[0]))
		expanded = ft_strdup("");
	else
		expanded = find_env_value(ms->envp, key, key_len);
	if (!expanded)
	{
		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
		return;
	}
	append_to_result(result, expanded, ms);
}