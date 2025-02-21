
#include "../../include/minishell.h"

t_bool	is_redirect(t_token_type type)
{
	if ((type == IN) || (type == OUT) || \
			(type == HEREDOC) || (type == APPEND))
		return (true);
	return (false);
}

/*Если тип этого элемента - редирект - сохраняем конкретный тип
	и переходим к следующему
  Если уже встретился ранее редир и не было слова:
	если пробелы - пропускаем и переходим, сбрасываем тип
	если слова - ставим им флаг и переходим, сбрасываем тип
  Если не встретился - просто переходим, сбрасываем тип
*/

void	flags_for_redirections(t_token *cur)
{
	t_token_type	specific;

	specific = EMPTY;
	while (cur)
	{
		if (is_redirect(cur->type)) //this one is redir
		{
			specific = cur->type;
			cur = cur->next;
			continue ; //don't need to stay on this one in a loop
		}
		if (is_redirect(specific)) //we met redir earlier
		{
			if (cur && cur->type == SPACE)
				cur = cur->next;
			while (cur && cur->type == WORD) //case with 2 words in a row without spaces?? "a"b??
			{
				cur->specific_redir = specific;
				cur = cur->next;
			}
			specific = EMPTY; //try case with 2 redirections in a row
		}
		else //we didn't meet redir
			cur = cur->next;
	}
}

void	put_files_for_redirections(t_token *cur)
{
	t_token	*deleted;


	while (cur)
	{
		if (is_redirect(cur->type) && cur->next && cur->next->type == WORD)
		{
			deleted = cur->next;
			cur->ambiguous = cur->next->ambiguous;
			if (!cur->ambiguous)
			{
				cur->file = ft_strdup(cur->next->data);
				if (deleted->file)
					free(deleted->file);
			}
			else
			{
				cur->file = cur->next->file; //check it's created
				free(deleted->data);
			}
			//free(deleted); //we have the node itself and 2 allocated fields
			cur->quote = cur->next->quote;
			cur->next = cur->next->next;
			//free(deleted->data);
			//free(deleted->file);
			free(deleted); //we have the node itself and 2 allocated fields
		} //and we free 2 of them which we don't need
		//printf("Type: %d, Data: %s, Quotes: %c, Redir: %d, Ambig: %d, File: %s\n", cur->type, cur->data, cur->quote, cur->specific_redir, cur->ambiguous, cur->file);
		cur = cur->next;
	}
}
