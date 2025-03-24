#include "../../include/minishell.h"

/**
 * @brief Converts an integer index to a string of digits.
 * 
 * This function converts an integer `index` to its string representation 
 * and stores it in the provided `buffer`. The function appends the converted 
 * digits to the buffer starting from the current position indicated by `len`. 
 * It handles the special case where the index is 0, storing `'0'` in the 
 * buffer. The resulting string is null-terminated at the end.
 * 
 * @param index The integer index to be converted into a string of digits.
 * @param buffer A pointer to the buffer where the resulting string will be 
 * stored. The buffer is modified in-place.
 * @param len A pointer to the current length of the buffer. This value is 
 * updated as the string is added to the buffer.
 * 
 * @return This function does not return; it modifies the provided `buffer` 
 * and updates the `len` value.
 */
static void	convert_index_to_digits(int index, char *buffer, int *len)
{
	char	digits[10];
	int		digit_len;
	int		i;

	digit_len = 0;
	if (index == 0)
	{
		buffer[*len] = '0';
		(*len)++;
		buffer[*len] = '\0';
		return ;
	}
	while (index > 0)
	{
		digits[digit_len++] = '0' + (index % 10);
		index /= 10;
	}
	i = digit_len - 1;
	while (i >= 0)
	{
		buffer[(*len)++] = digits[i];
		i--;
	}
	buffer[(*len)] = '\0';
}

/**
 * @brief Initializes a buffer with a predefined string.
 * 
 * This function initializes a given `buffer` by appending the string 
 * `.here_doc_` at the current position indicated by `len`. Each character 
 * of the string is added one by one, and the `len` is updated to reflect 
 * the new length of the buffer after each addition.
 * 
 * @param buffer A pointer to the buffer to be initialized. The buffer is 
 * modified in-place.
 * @param len A pointer to the current length of the buffer. This value is 
 * updated as characters are added to the buffer.
 * 
 * @return This function does not return; it modifies the provided `buffer` 
 * and updates the `len` value.
 */
static void	init_buffer(char *buffer, int *len)
{
	buffer[(*len)++] = '.';
	buffer[(*len)++] = 'h';
	buffer[(*len)++] = 'e';
	buffer[(*len)++] = 'r';
	buffer[(*len)++] = 'e';
	buffer[(*len)++] = 'd';
	buffer[(*len)++] = 'o';
	buffer[(*len)++] = 'c';
	buffer[(*len)++] = '_';
}

/**
 * @brief Initializes a buffer by appending a predefined string.
 * 
 * This function appends the string `.here_doc_` to a given `buffer`, 
 * starting from the current position indicated by `len`. The `len` is 
 * updated after each character is added, reflecting the new length of 
 * the buffer after the string is appended.
 * 
 * @param buffer A pointer to the buffer to be initialized. The buffer is 
 * modified in-place.
 * @param len A pointer to the current length of the buffer. This value is 
 * updated as characters are added.
 * 
 * @return This function does not return a value; it modifies the provided 
 * `buffer` and updates the `len` value.
 */
char	*generate_filename(int index, t_ms *ms)
{
	char	*filename;
	char	buffer[20];
	int		len;

	len = 0;
	init_buffer(buffer, &len);
	convert_index_to_digits(index, buffer, &len);
	filename = ft_strdup(buffer);
	if (!filename)
	{
		print_malloc_error();
		ms->exit_status = MALLOC_ERR;
		return (NULL);
	}
	return (filename);
}
