#include "../../include/minishell.h"

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
		return (NULL); //protect malloc
	ft_strncpy(before, str, len);
	before[len] = '\0';
	return (before);
}

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

int	has_multiple_words(const char *str)
{
	int	i = 0;
	int	found_word;

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
