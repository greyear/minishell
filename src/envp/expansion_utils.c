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
    free(key);
    key = check;
    res = find_env_value(envp, key, len);
    free(key);
    return (res);
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