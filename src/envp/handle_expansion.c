/*
handles $USER, so enviroment variable expansion
args has all the arrays need to bee expanded
for example args = "My home directory is $HOME" or args="$USER$LOGNAME" or args="THis is $INVALIDKEY"
"My home directory is /Users/seela", "seelasalorintaseela", "THis is  "
if the KEY dont exists replace the $HOME with "", else replace $HOME with the VALUE

KEY only contains numbers, alphabets or _ and the first character cant be a number

edge cases if KEY is $? , replace the KEY with latest exit_value
"This is $?" = "This is 127"
edge case if KEY is $$, replace the KEy with pid
"This is $$" "This is 1140" for example
*/

#include "../../include/minishell.h"

static char	*verify_key(char *key)
{
	if (ft_isdigit(key[0])) // If the key starts with a digit, it's invalid
		return (NULL);
	return (ft_strdup(key));
}

char	*expand_key(char **envp, char *key, int len, t_ms *ms)
{
	int		i;
	char	*check;
	char	*res;

	i = 0;
	if (key[0] == '?')
		return(ft_itoa(ms->exit_status));
	if (key[0] == '$')
		return (ft_itoa(getpid()));
	check = verify_key(key);
	if (!check)
		return (ft_strdup(""));
	else
		key = check;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, len) == 0)
		{
			if (envp[i][len] == '=')
			{
				res = ft_strdup(envp[i] + len + 1); //fixed a bit bc of a leak
				free(key);
				return (res);
			}
				
		}
		i++;
	}
	res = ft_strdup("");
	free(key);
	return (res);
}

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
	*i += x; // Move i forward by the length of the key
	return (key);
}

char	*expand_variable(t_ms *ms, char *key, int key_len)
{
	char	*expanded_value;

	expanded_value = expand_key(ms->envp, key, key_len, ms);
	free(key);
	return (expanded_value);
}

void	append_to_result(char **result, char *new_part)
{
	char	*temp;

	temp = ft_strjoin(*result, new_part);
	free(*result);
	*result = temp;
	free(new_part);
}

void	append_literal_char(char **result, char c)
{
	char	*substr;
	char	*temp;
	char	str[2];

	str[0] = c;
	str[1] = '\0';
	substr = ft_substr(str, 0, 1);  // Convert char to string
	temp = ft_strjoin(*result, substr);
	free(substr);
	free(*result);
	*result = temp;
}

char	*handle_expansion(char *args, t_ms *ms)
{
	int		i;
	char	*key;
	char	*expanded;
	char	*result;

	result = ft_strdup("");
	i = 0;
	while (args[i])
	{
		if (args[i] == '$' && args[i + 1] && !ft_isspace(args[i + 1]))
		{
			i++; // Move past '$'
			key = extract_key_export(args, &i);
			expanded = expand_variable(ms, key, ft_strlen(key));
			append_to_result(&result, expanded);
		}
		else
			append_literal_char(&result, args[i++]);
	}
	return (result);
}
