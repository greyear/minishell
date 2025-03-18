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
					//free(copy);
					if (exp->if_first && without_spaces[0] == ' ')
					{
						without_first = remove_first_space(without_spaces);
						//free(without_spaces);
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
	append_to_result(result, expanded);
}

char	*remove_extra_spaces(char *str)
{
	char	*new;
	int		i;
	int		j;
	int		space;
	int len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	new = ft_calloc(len + 1, sizeof(char));
	if (!new)
		return (NULL);
	i = 0;
	j = 0;
	space = 0; //1 if met space
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
	new[j] = '\0';
	free(str);
	return (new);
}

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
