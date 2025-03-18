
#include "../../include/minishell.h"

t_expand	*exp_init(void)
{
	t_expand	*exp;

	exp = malloc(sizeof(t_expand));
	if (!exp)
	{
		perror("memory allocation failed");
		exit(1); //?
	}
	exp->data = NULL;
	exp->key = NULL;
	exp->len = 0;
	exp->quote = 0;
	exp->if_first = 0;
	return (exp);
}

int	expand_in_token(t_token *cur, t_ms *ms, t_bool first_in_str)
{
	char	*data_copy;
	char	*expanded;
	t_expand	*exp;

	data_copy = ft_strdup(cur->data);
	if (!data_copy)
		return (1); //error msg?
	exp = exp_init();
	//check for malloc
	if (ft_strcmp(cur->data, "$") == 0 && !cur->quote && cur->next && cur->next->quote)
		expanded = ft_strdup("");
	else
	{
		exp->data = cur->data;
		exp->quote = cur->quote;
		exp->if_first = first_in_str;
		expanded = handle_expansion(exp, ms);
	}
		
	if (!expanded)
	{
		free(data_copy);
		free(exp);
		return (1);
	}
	free(cur->data);
	cur->data = expanded;

	if (cur->specific_redir && !cur->quote && \
			data_copy[0] && !cur->data[0])
	{
		cur->ambiguous = true;
		cur->file = data_copy;
		//printf("new cur->file: %s\n", cur->file);
	}
	//what if after expanding it became empty?
	else
		free(data_copy);
	free(exp);
	return (0);
}

int	check_list_for_expansions(t_token *first, t_ms *ms)
{
	t_token	*cur;
	t_bool	first_in_str;

	cur = first;
	//check r
	first_in_str = 1;
	while (cur) //heredoc?
	{
		if (cur->type == WORD && cur->quote != SG_QUOT
			&& cur->specific_redir != HEREDOC) //sg quoted '$HOME' shouldn't be expanded, word after << symbol also shouldn't
		{
			//printf("before expanding, %s\n", cur->data);
			if (expand_in_token(cur, ms, first_in_str) == 1)
				return (1);
			//first_in_str = 0;
		}
		if (cur->type == WORD)
			first_in_str = 0;
		if (cur->type != WORD)
			first_in_str = 1;
		cur = cur->next;
	}
	return (0);
}

int	expand_tilde(t_token *cur, t_ms *ms)
{
	char	*home;
	char	*new_data;

	if (!cur->data || cur->data[0] != '~') // Ensure `~` is at the start
		return (0);
	home = get_home_directory(ms, 1);
	if (!home)
		return (0);
	if (cur->data[1] == '\0') // If `~` is alone
		new_data = ft_strdup(home);
	else if (cur->data[1] == '/') // If `~/something`
		new_data = ft_strjoin(home, &cur->data[1]);
	else
	{
		free(home);
		return (0); // If `~` is part of a word like `hello~`, don't expand
	}
	free(home);
	if (!new_data)
		return (1); // Memory allocation error
	free(cur->data);
	cur->data = new_data;
	return (0);
}

int	check_list_for_tilde(t_token *first, t_ms *ms)
{
	t_token	*cur;

	cur = first;
	while (cur)
	{
		if (cur->type == WORD && cur->quote != SG_QUOT && cur->quote != DB_QUOT
			&& cur->data[0] == '~') // Ensuring `~` is at the start
		{
			if (expand_tilde(cur, ms) == 1)
				return (1);
		}
		cur = cur->next;
	}
	return (0);
}


/*
heredoc explanation for myself:

Here document (Heredoc) is a type of redirection that allows you to pass 
multiple lines of input to a command. The most commonly used delimiters are
 EOF or END.

[COMMAND] <<[-] 'DELIMITER'
  HERE-DOCUMENT
DELIMITER

e.g.
cat << 'EOF'
echo $HOME
EOF

Special cases: when delimiter is in SG Quotes
				<<- cuts tabs?
				if $HOME is put instead of EOF: the shell will wait for 
				the same $HOME to end the lines, not the expanded value - 
				SO we don't need to expand $HOME in this case!
*/