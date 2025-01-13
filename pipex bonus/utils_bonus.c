/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azinchen <azinchen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 19:57:21 by azinchen          #+#    #+#             */
/*   Updated: 2024/12/18 19:57:25 by azinchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	init_p(t_pipex	*p)
{
	p->cmds = NULL;
	p->cmd_num = 0;
	p->cur_fd = -1;
}

void	close_fds(int fd1, int fd2)
{
	if (fd1 != -1)
		close(fd1);
	if (fd2 != -1)
		close(fd2);
}

int	length_inside_quotes(char *str, t_pipex *p, char *cmd)
{
	char	any_quote;
	int		len;

	if (!str || (*str != '\'' && *str != '\"'))
		return (0);
	any_quote = *str;
	len = 0;
	str++;
	while (*str != '\0' && *str != any_quote)
	{
		len++;
		str++;
	}
	if (*str != any_quote)
		cmd_error(CMD_NOT_FOUND, cmd, CMD_NF, &p);
	return (len);
}

int	word_length(char *str)
{
	int	len;

	len = 0;
	while (*str && *str != ' ' && *str != '\'' && *str != '\"')
	{
		if (*str == '\\' && *(str + 1) != '\0')
		{
			len += 2;
			str += 2;
		}
		else
		{
			len++;
			str++;
		}
	}
	return (len);
}
