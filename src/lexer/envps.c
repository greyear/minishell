
#include "../../include/minishell.h"

t_bool	is_envp_symbol(int c)
{
	if (ft_isalnum(c) || c == UNDERSC)
		return (true);
	return (false);
}

t_envp	*envp_from_list(t_envp *list, char *name)
{
	t_envp	*cur;

	if (!list)
		return (NULL); //maybe we don't need it here
	cur = list;
	while (cur)
	{
		if (ft_strncmp(cur->name, name, ft_strlen(name)) == 0) //change for strcmp!
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


envps:
✅ Allowed Characters in Environment Variable Names
Uppercase Letters: A-Z
Digits (0-9) – but NOT at the start
Underscore (_)
❌ Not Allowed
Lowercase letters (a-z) → Though technically allowed, they are usually not used by convention.
Special Characters (!@#$%^&*()-+=,.:;'"<>?[]{})
Spaces → MY VAR=hello is invalid.
Starting with a Number → 1HOME=/root is invalid.
*/