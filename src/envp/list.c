
#include "../../include/minishell.h"

t_envp	*envp_from_list(t_envp *list, char *name)
{
	t_envp	*cur;

	cur = list;
	while (cur)
	{
		if (ft_strcmp(cur->name, name) == 0)
			return (cur);
		cur = cur->next;
	}
	return (NULL);
}


/*
get_key(str, start, end) = ft_substr(str, start, end - start)
remove_string_section - cuts the part from the given string and returns this string without this part
add_content - (aux) inserts content (input) to the specific position (input index) in string
add_string_to - allocates memory and puts content (input) to the specific position (input index) in string
expand_return_value: when we call $? to get the exit value of prev command
expand_env: 
*/

