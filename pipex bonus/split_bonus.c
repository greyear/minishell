/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azinchen <azinchen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 19:59:25 by azinchen          #+#    #+#             */
/*   Updated: 2024/12/18 19:59:29 by azinchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static int	count_words(char *str, t_pipex *p)
{
	int	i;
	int	res;

	if (!str || str[0] == '\0')
		return (0);
	res = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			res++;
			i += length_inside_quotes(&str[i], p, str) + 2;
		}
		else if (str[i] != ' ')
		{
			res++;
			i += word_length(&str[i]);
		}
		else
			i++;
	}
	return (res);
}

static char	*one_word(char *str, int len)
{
	int		i;
	int		j;
	char	*word;

	word = (char *)ft_calloc((len + 1), sizeof(char));
	if (!word)
		return (NULL);
	i = 0;
	j = 0;
	while (i < len)
	{
		if (ft_isquote(str[0]) && str[i] == str[0])
			i++;
		else if (!ft_isquote(str[0]) && (str[i] == ' ' || str[i] == '\\'))
			i++;
		else
			word[j++] = str[i++];
	}
	word[j] = '\0';
	return (word);
}

static char	**all_words(char *str, int count, t_pipex *p)
{
	int		w;
	int		len;
	char	**array;

	array = (char **)ft_calloc((count + 1), sizeof(char *));
	if (!array)
		return (NULL);
	w = -1;
	while (*str && ++w < count)
	{
		if (*str == ' ')
			str++;
		if (ft_isquote(*str))
			len = length_inside_quotes(str, p, str) + 2;
		else
			len = word_length(str);
		array[w] = one_word(str, len);
		if (!array[w])
		{
			clean_arr(&array);
			return (NULL);
		}
		str += len;
	}
	return (array);
}

char	**split_cmd(char *cmd, t_pipex *p)
{
	char	**res;
	int		count;

	if (!cmd || *cmd == '\0')
		return (NULL);
	count = count_words(cmd, p);
	if (count == 0)
		return (NULL);
	res = all_words(cmd, count, p);
	if (!res)
		return (NULL);
	return (res);
}
