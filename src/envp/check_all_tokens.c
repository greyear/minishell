
#include "../../include/minishell.h"

int	expand_in_token(t_token *cur, t_ms *ms)
{
	char	*data_copy;

	data_copy = ft_strdup(cur->data);
	if (!data_copy)
		return (1); //error msg?
	cur->data = handle_expansion(cur->data, ms);
	if (!cur->data)
	{
		free(data_copy);
		return (1);
	}
	//what if after expanding it became empty?
	//what if we expanded redir filename and it became empty?
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
		if (cur->type == WORD && cur->quote != SG_QUOT && \
				cur->specific_redir != HEREDOC) //sg quoted '$HOME' shouldn't be expanded, word after << symbol also shouldn't
		{
			if (expand_in_token(cur, ms) == 1)
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