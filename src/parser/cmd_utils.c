#include "../../include/minishell.h"

/**
 * @brief Extracts the substring before the first space in a given 
 * string.
 * 
 * This function scans the input string `str` until it encounters 
 * a space or reaches the end of the string. It then allocates memory 
 * for a new string, copies the characters before the first space 
 * into it, and null-terminates it.
 * 
 * @param str The input string from which the substring is extracted.
 * 
 * @return A newly allocated string containing the characters before 
 *         the first space, or `NULL` if memory allocation fails or 
 *         if `str` is `NULL`.
 */
char	*str_before_space(const char *str)
{
	int		len;
	char	*before;

	if (!str)
		return (NULL);
	len = 0;
	while (str[len] && str[len] != ' ')
		len++;
	before = (char *)malloc(len + 1);
	if (!before)
		return (NULL);
	ft_strncpy(before, str, len);
	before[len] = '\0';
	return (before);
}

/**
 * @brief Returns a newly allocated string containing the part after 
 * the first space.
 * 
 * This function searches for the first occurrence of a space in the 
 * given string `str`. If found, it returns a duplicate of the 
 * substring that comes after the space. If there is no space or the 
 * space is at the end of the string, the function returns `NULL`.
 * 
 * @param str The input string to process.
 * 
 * @return A newly allocated string containing the substring after 
 *         the first space, or `NULL` if there is no valid substring 
 *         or memory allocation fails.
 */
char	*str_after_space(const char *str)
{
	char	*space_ptr;

	if (!str)
		return (NULL);
	space_ptr = ft_strchr(str, ' ');
	if (!space_ptr || *(space_ptr + 1) == '\0')
		return (NULL);
	return (ft_strdup(space_ptr + 1));
}

/**
 * @brief Checks if a string contains more than one word.
 * 
 * This function determines whether the given string `str` contains 
 * at least two words separated by spaces. A word is defined as 
 * a sequence of non-space characters. The function ignores leading 
 * and intermediate spaces but ensures that at least one space 
 * separates two words.
 * 
 * @param str The input string to check.
 * 
 * @return `1` if the string contains more than one word, `0` 
 *         otherwise.
 */
int	has_multiple_words(const char *str)
{
	int	i;
	int	found_word;

	i = 0;
	found_word = 0;
	if (!str)
		return (0);
	while (str[i] == ' ')
		i++;
	while (str[i] && str[i] != ' ')
	{
		found_word = 1;
		i++;
	}
	while (str[i] == ' ')
		i++;
	return (found_word && str[i] != '\0');
}

/**
 * @brief Copies a single word to the command's argument array.
 * 
 * @param cmd Pointer to the command structure where the argument is stored.
 * @param data The string to be copied.
 * @param index Pointer to the current index in the `args` array.
 * @param ms Pointer to the shell structure for error handling.
 * 
 * @return `1` on allocation failure, `0` on success.
 */
int	copy_single_word(t_cmd *cmd, const char *data, int *index, t_ms *ms)
{
	cmd->args[*index] = ft_strdup(data);
	if (!cmd->args[*index])
	{
		print_malloc_set_status(ms);
		return (1);
	}
	(*index)++;
	return (0);
}

/**
 * @brief Splits an expanded word and stores both parts in the command's 
 * argument array.
 * 
 * @param cmd Pointer to the command structure where the arguments are stored.
 * @param data The expanded string to be split.
 * @param index Pointer to the current index in the `args` array.
 * @param ms Pointer to the shell structure for error handling.
 * 
 * @return `1` on allocation failure, `0` on success.
 */
int	copy_expanded_words(t_cmd *cmd, const char *data, int *index, t_ms *ms)
{
	cmd->args[*index] = str_before_space(data);
	cmd->args[*index + 1] = str_after_space(data);
	if (!cmd->args[*index] || !cmd->args[*index + 1])
	{
		print_malloc_set_status(ms);
		return (1);
	}
	*index += 2;
	return (0);
}
