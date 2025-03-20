#include "../../include/minishell.h"

/**
 * @brief Calculates the length of a key in a key-value pair string.
 *
 * This function determines the length of the key portion in a string 
 * formatted as "key=value". It stops counting at the first occurrence 
 * of the '=' character or at the end of the string.
 *
 * @param arg The string containing the key-value pair.
 * @return The length of the key before the '=' character.
 */

int	get_key_length(char *arg)
{
	int		len;

	len = 0;
	while (arg[len] && arg[len] != '=')
		len++;
	return (len);
}

/**
 * @brief Extracts the key from a key-value pair string.
 *
 * This function allocates memory and copies the key portion from a string 
 * formatted as "key=value". The length of the key must be provided.
 *
 * @param arg The string containing the key-value pair.
 * @param len The length of the key before the '=' character.
 * @return A newly allocated string containing the extracted key, 
 *         or NULL on failure.
 * The caller is responsible for freeing the allocated memory.
 */

char	*extract_key(char *arg, int len)
{
	char	*name;

	name = malloc(sizeof(char) * (len + 1));
	if (!name)
	{
		print_malloc_error();
		return (NULL);
	}
	ft_strncpy(name, arg, len);
	name[len] = '\0';
	return (name);
}

/**
 * @brief Checks if a given string is a valid environment variable key.
 *
 * A valid key must:
 * - Not be NULL or empty.
 * - Not start with a digit.
 * - Contain only alphanumeric characters or underscores.
 *
 * @param name The string to validate as a key.
 * @return 0 if the key is invalid, 1 if it is valid.
 */

int	check_if_valid_key(char *name)
{
	int		x;

	x = 0;
	if (!name || !*name)
		return (0);
	if (name[0] >= '0' && name[0] <= '9')
		return (0);
	while (name[x])
	{
		if (!ft_isalnum(name[x]) && name[x] != '_')
			return (0);
		x++;
	}
	return (1);
}
