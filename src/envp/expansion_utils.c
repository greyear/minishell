#include "../../include/minishell.h"
 
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
            if (envp[i][len] == '=')
                return (ft_strdup(envp[i] + len + 1));
        }
        i++;
    }
    return (ft_strdup(""));
}

/**
 * @brief Expands an environment variable key to its corresponding value.
 *
 * This function retrieves the value of an environment variable given its key. 
 * If the key is `"?"`, it returns a string representation of `ms->exit_status`.
 * If the key starts with a digit, an empty string is returned. Otherwise, 
 * it searches for the key in the environment variables.
 *
 * @param ms Pointer to the main shell structure containing `exit_status` and `envp`.
 * @param key The environment variable key (dynamically allocated).
 * @param key_len The length of the key.
 *
 * @return A newly allocated string containing the variable's value, an empty string if 
 *         the key starts with a digit, or `NULL` if no match is found.
 *         The caller is responsible for freeing the returned string.
 *
 * @note The function frees the `key` argument before returning.
 */
 
char	*expand_variable(t_ms *ms, char *key, int key_len)
{
    char	*expanded_value;
 
    if (!key || !*key)
        return (NULL);
    if (key[0] == '?')
        return ft_itoa(ms->exit_status);
    if (ft_isdigit(key[0]))
        return (ft_strdup(""));
    expanded_value = find_env_value(ms->envp, key, key_len);
    free(key);
    return (expanded_value);
}