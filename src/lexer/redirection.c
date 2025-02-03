
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
			continue ;
		}
		if (is_redirect(specific)) //we met redir earlier
		{
			if (cur && cur->type == SPACE)
				cur = cur->next;
			while (cur && cur->type == WORD)
			{
				cur->specific_redir = specific;
				cur = cur->next;
			}
		}
		else //we didn't meet redir
			cur = cur->next;
		specific = EMPTY;
	}
}
