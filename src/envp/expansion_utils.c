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

static char	*find_env_value(char **envp, t_expand *exp)
{
	int		i;
	char	*copy;
	char	*without_spaces;
	char	*without_first;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], exp->key, exp->len) == 0)
		{
			if (envp[i][exp->len] && envp[i][exp->len] == '=')
			{
				copy = ft_strdup(envp[i] + exp->len + 1);
				if (!copy)
					return (NULL); //malloc error
				if (exp->quote == 0) //for heredocs????
				{
					without_spaces = remove_extra_spaces(copy);
					if (exp->if_first && without_spaces[0] == ' ')
					{
						without_first = remove_first_space(without_spaces);
						return (without_first);
					}
					return (without_spaces);
				}
				else
					return (copy);
			}
		}
		i++;
	}
	return (ft_strdup(""));
}

void	expand_variable(t_ms *ms, t_expand *exp, char **result)
{
	char	*expanded;

	if (!exp->key || !*(exp->key))
		return;
	if (exp->key[0] == '?')
		expanded = ft_itoa(ms->exit_status);
	else if (ft_isdigit(exp->key[0]))
		expanded = ft_strdup("");
	else
		expanded = find_env_value(ms->envp, exp);
    if (!expanded)
    {
      print_malloc_error();
      ms->exit_status = MALLOC_ERR;
      return;
    }
	append_to_result(result, expanded, ms);
}

/**
 * @brief Skips consecutive spaces in the input string and copies the result to a new string.
 * 
 * This function iterates through the input string, copying characters to the `new` string. It ensures
 * that only a single space character is retained between words, removing any consecutive spaces from the 
 * input. If a space is encountered after a previous space, it is skipped. Non-space characters are copied 
 * directly to the new string. The resulting string will not have leading, trailing, or consecutive spaces.
 * 
 * @param str The input string to be processed.
 * @param new A new string that will hold the result with extra spaces removed.
 * 
 * @return Returns the length of the processed string.
 */
static int	skip_spaces(const char *str, char *new)
{
	int i;
	int j;
	int space;

	i = 0;
	j = 0;
	space = 0;
	while (str[i])
	{
		if (ft_isspace(str[i]) && !space) // Combine the conditions
		{
			new[j++] = ' ';
			space = 1;
		}
		else if (!ft_isspace(str[i])) // Handle non-space characters
		{
			new[j++] = str[i];
			space = 0;
		}
		i++;
	}
	new[j] = '\0';
	return (j);
}

/**
 * @brief Removes extra spaces from the input string, leaving only single spaces between words.
 * 
 * This function processes the input string by removing any consecutive spaces, replacing them
 * with a single space. If there are no spaces, it returns the original string unchanged. The function
 * ensures that spaces around environment variable expansions are preserved. After processing, the 
 * original string is freed and a new string with reduced spaces is returned.
 * 
 * @param str The input string to be processed.
 * 
 * @return A new string with reduced spaces between words, or the original string if no extra spaces
 *         were found. Returns NULL if memory allocation fails.
 */
char	*remove_extra_spaces(char *str)
{
	char	*new;

	if (!str)
		return (NULL);
	new = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!new)
		return (NULL);
	skip_spaces(str, new);
	free(str);
	return (new);
}

/**
 * @brief Removes the first space from the given string if it exists.
 * 
 * This function checks if the input string starts with a space. If it does, it creates
 * a new string without the leading space and frees the original string. If the string 
 * does not start with a space or is NULL, it returns the original string unchanged.
 * 
 * @param str The input string to be processed.
 * 
 * @return A new string without the leading space if a space was present, otherwise 
 *         returns the original string. Returns NULL if memory allocation fails.
 */
char	*remove_first_space(char *str)
{
	char	*new;
	int		i;

	if (!str || str[0] != ' ')
		return (str);
	i = 1;
	new = ft_strdup(str + i);
	if (!new)
		return (NULL);
	free(str);
	return (new);
}
