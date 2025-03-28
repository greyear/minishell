/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spaces_in_expansion.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 13:36:25 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 13:36:27 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Skips consecutive spaces in the input string and copies the result 
 *        to a new string.
 * 
 * This function iterates through the input string, copying characters to the 
 * `new` string. It ensures that only a single space character is retained 
 * between words, removing any consecutive spaces from the input. If a space 
 * is encountered after a previous space, it is skipped. Non-space characters 
 * are copied directly to the new string. The resulting string will not have 
 * leading, trailing, or consecutive spaces.
 * 
 * @param str The input string to be processed.
 * @param new A new string that will hold the result with extra spaces removed.
 * 
 * @return Returns the length of the processed string.
 */
static int	skip_spaces(const char *str, char *new)
{
	int		i;
	int		j;
	int		space;

	i = 0;
	j = 0;
	space = 0;
	while (str[i])
	{
		if (ft_isspace(str[i]) && !space)
		{
			new[j++] = ' ';
			space = 1;
		}
		else if (!ft_isspace(str[i]))
		{
			new[j++] = str[i];
			space = 0;
		}
		i++;
	}
	new[j] = '\0';
	return (j);
}

/**
 * @brief Removes extra spaces from the input string, leaving only single 
 *        spaces between words.
 * 
 * This function processes the input string by removing any consecutive spaces, 
 * replacing them with a single space. If there are no spaces, it returns the 
 * original string unchanged. The function ensures that spaces around 
 * environment variable expansions are preserved. After processing, the original
 * string is freed and a new string with reduced spaces is returned.
 * 
 * @param str The input string to be processed.
 * 
 * @return A new string with reduced spaces between words, or the original
 *         string if no extra spaces were found. Returns NULL if memory 
 *         allocation fails.
 */
static char	*remove_extra_spaces(char *str)
{
	char	*new;

	if (!str)
		return (NULL);
	new = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!new)
		return (NULL);
	skip_spaces(str, new);
	free(str);
	return (new);
}

/**
 * @brief Removes the first space from the given string if it exists.
 * 
 * This function checks if the input string starts with a space. If it does, it 
 * creates a new string without the leading space and frees the original string. 
 * If the string does not start with a space or is NULL, it returns the original 
 * string unchanged.
 * 
 * @param str The input string to be processed.
 * 
 * @return A new string without the leading space if a space was present, 
 *         otherwise returns the original string. Returns NULL if memory 
 *         allocation fails.
 */
static char	*remove_first_space(char *str)
{
	char	*new;
	int		i;

	if (!str || str[0] != ' ')
		return (str);
	i = 1;
	new = ft_strdup(str + i);
	if (!new)
		return (NULL);
	free(str);
	return (new);
}

/**
 * @brief Removes extra spaces from a string and optionally removes 
 *        a leading space.
 * 
 * This function processes the input string by removing redundant spaces. 
 * If `exp->if_first` is set and the resulting string starts with a space, 
 * the first space is also removed. The function returns a newly allocated 
 * string, and the caller is responsible for freeing it.
 * 
 * @param copy The input string to be processed.
 * @param exp A pointer to the `t_expand` structure, which determines whether 
 *            to remove a leading space.
 * 
 * @return A dynamically allocated string with extra spaces removed. 
 *         Returns `NULL` if memory allocation fails.
 */
char	*handle_spaces(char *copy, t_expand *exp)
{
	char	*without_spaces;
	char	*without_first;

	without_spaces = remove_extra_spaces(copy);
	if (!without_spaces)
		return (NULL);
	if (exp->if_first && without_spaces[0] == ' ')
	{
		without_first = remove_first_space(without_spaces);
		return (without_first);
	}
	return (without_spaces);
}
