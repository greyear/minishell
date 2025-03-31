/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 12:56:39 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 12:56:41 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	char	*key;

	key = malloc(sizeof(char) * (len + 1));
	if (!key)
		return (NULL);
	ft_strncpy(key, arg, len);
	key[len] = '\0';
	return (key);
}

/**
 * @brief Checks if a given string is a valid environment variable key.
 *
 * A valid key must:
 * - Not be NULL or empty.
 * - Not start with a digit.
 * - Contain only alphanumeric characters or underscores.
 *
 * @param key The string to validate as a key.
 * @return 0 if the key is invalid, 1 if it is valid.
 */
int	check_if_valid_key(char *key)
{
	int		x;

	x = 0;
	if (!key || !*key)
		return (0);
	if (key[0] >= '0' && key[0] <= '9')
		return (0);
	while (key[x])
	{
		if (!ft_isalnum(key[x]) && key[x] != '_')
			return (0);
		x++;
	}
	return (1);
}
