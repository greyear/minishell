
#include "../../include/minishell.h"

int	expand_in_token(t_token *cur, t_ms *ms)
{
	char	*data_copy;
	char	*expanded;

	data_copy = ft_strdup(cur->data);
	if (!data_copy)
		return (1); //error msg?

	if (ft_strcmp(cur->data, "$") == 0 && cur->next && cur->next->quote)
		expanded = ft_strdup("");
	else
		expanded = handle_expansion(cur->data, ms);
	if (!expanded)
	{
		free(data_copy);
		return (1);
	}
	free(cur->data);
	cur->data = expanded;
	//printf("new cur->data: %s\n", cur->data);
	//printf("new cur->data: %s\n", cur->data);
	if (cur->specific_redir && !cur->quote && \
			data_copy[0] && !cur->data[0])
	{
		cur->ambiguous = true;
		cur->file = data_copy;
		//printf("new cur->file: %s\n", cur->file);
		//printf("new cur->file: %s\n", cur->file);
	}
	//what if after expanding it became empty?
	else
		free(data_copy);
	return (0);
}

int	check_list_for_expansions(t_token *first, t_ms *ms)
{
	t_token	*cur;

	cur = first;
	//check r
	while (cur) //heredoc?
	{
		if (cur->type == WORD && cur->quote != SG_QUOT
			&& cur->specific_redir != HEREDOC) //sg quoted '$HOME' shouldn't be expanded, word after << symbol also shouldn't
		{
			if (expand_in_token(cur, ms) == 1)
				return (1);
		}
		cur = cur->next;
	}

	/*printf("\nbefore deleting whitespaces\n");
	print_tokens(first);*/

	//first = delete_whitespace_tokens(first);

	/*printf("\nafter deleting whitespaces\n");
	print_tokens(first);*/

	//first = delete_empty_word_tokens(first);

	/*printf("\nafter deleting empty\n");
	print_tokens(first);*/

	return (0);
}


int	expand_tilde(t_token *cur, t_ms *ms)
{
	char *home;
	char *new_data;
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
	if (!cur->data || cur->data[0] != '~') // Ensure `~` is at the start
        return (0);
    home = get_home_directory(ms, 1);
    if (!home)
        return (0);
    if (cur->data[1] == '\0')
        new_data = ft_strdup(home);
    else if (cur->data[1] == '/')
        new_data = ft_strjoin(home, &cur->data[1]);
	else
	{
		free(home);
		return (0);
	}
	free(home);
	if (!new_data)
		return (1);
    free(cur->data);
    cur->data = new_data;
    return (0);
}

int	check_list_for_tilde(t_token *first, t_ms *ms)
{
	t_token *cur = first;

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