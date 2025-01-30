
#include "../../include/minishell.h"

t_token_type	define_token_type(char *str, size_t i)
{
	//&?
	if (str[i] == '<' && str[i + 1] == '<')
		return (HEREDOC);
	else if (str[i] == '<')
		return (IN);
	else if (str[i] == '>' && str[i + 1] == '>')
		return (APPEND);
	else if (str[i] == '>')
		return (OUT);
	else if (str[i] == '|') // do we need to check the second | or if we already checked it in BNF it's unnecessary?
		return (PIPE);
	else if (str[i] == '(')
		return (LPAR);
	else if (str[i] == ')')
		return (RPAR);
	else if (ft_isspace(str[i]))
		return (SPACE);
	else
		return (WORD);
}

static void	default_values(t_token *new)
{
	new->data = NULL;
	new->file = NULL;
	new->next = NULL;
	new->quote = NULL;
	new->unclosed = false; //check in debugger if we need it
	//some other fields
}

t_token	*create_new_token(char *str, size_t *i, t_token_type type)
{
	t_token	*new;

	new = (t_token *)ft_calloc(1, sizeof(t_token));
	if (!new)
		return (NULL); //error?
	default_values(new);
	new->type = type;
	if (type == WORD)
	{
		if (str[*i] == SG_QUOT || str[*i] == DB_QUOT)
		{
			new->quote = str[*i];
			new->data = word_with_quotes(str, i, new);
		}
		else
			new->data = word_without_quotes(str, i);
		if (!new->data)
			return (NULL); //error?
	}
	else if (type == SPACE)
		skip_whitespaces(str, i);
	else
		skip_special_tokens(str, i, type);
	return (new);
}

//define type -> check size -> for all except spaces and words ++ -> for word ++ and put content
//


