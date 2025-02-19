
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
should have:
get_key(str, start, end) = ft_substr(str, start, end - start)
remove_string_section - cuts the part from the given string and returns this string without this part
add_content - (aux) inserts content (input) to the specific position (input index) in string
add_string_to - allocates memory and puts content (input) to the specific position (input index) in string
expand_return_value: when we call $? to get the exit value of prev command
find_env: goes through the env list and if the key is found - returns the whole env
fetch_env: calls find_env, finds matching env from the list, duplicates content by key
expand_env: gets key, finds content by key, cuts key and changes it for value, recursively calls for expand_envs_in_string
expand_envs_in_string: finds $ in string, handles $? separately and calls expand_env for other cases
expand_words_envs: saves content into buf, change content onto expanded one, sets the flag "empty_string", checks for redirection file name separately, frees buf
expand_envs: looks for words in the whole list (with some exceptions) and tries to expand it

SO: what we do is:
1) find $, handle $? case,
2)find value by key for expanding, insert value, 
3) if "$PATH" in db quotes - expand, if '$PATH' - DON'T.
   set flag if expanding returns NULL for redirection filename
   RECURSIVELY expand envs!

we now have:
verify_key: check the 1st symbol of the key, return NULL on error, duplicate of the key on success
			BUT: we don't check other symbols that cannot be used inside the key: "!, @, -, =" and some other symbols.
			maybe we can check allowed symbols instead (I wanted to implement the logic in letters.c file):
			Allowed Characters in Environment Variable Names
			Uppercase Letters: A-Z
			Digits (0-9) – but NOT at the start
			Underscore (_)

expand_key: $? returns exit status as it should.
			$$ returns hardcoded value, but should deal with processes??
			in case with ordinary env:
				checks the env list and returns the value
			if didn't find - returns empty line
handle_expansion:
			goes through the string and finds:
			$? / $HOME (checks symbols)
			joins string and returned value found by key
			check $1 case!

So potential problems are:

Check for infinite loop:
export A='$A'
echo $A

*/

