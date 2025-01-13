/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azinchen <azinchen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 20:02:36 by azinchen          #+#    #+#             */
/*   Updated: 2024/12/18 20:02:39 by azinchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	handle_command(char *cmd, t_pipex *p)
{
	char	*path;

	p->cmds = split_cmd(cmd, p);
	if (!p->cmds)
		cmd_error(CMD_NOT_FOUND, cmd, CMD_NF, &p);
	path = find_path(p->cmds, p);
	if (!path)
		cmd_error(CMD_NOT_FOUND, p->cmds[0], CMD_NF, &p);
	execve(path, p->cmds, p->envp);
	execve_fail(&path, p->cmds, &p);
}

char	**path_from_envp(t_pipex *p)
{
	int	i;

	if (!p->envp)
		return (NULL);
	i = 0;
	while (p->envp[i])
	{
		if (ft_strncmp(p->envp[i], "PATH=", 5) == 0)
			return (ft_split(p->envp[i] + 5, ':'));
		i++;
	}
	return (NULL);
}

static char	*make_full_path(char *one_path, char *cmd)
{
	char	*premade;
	char	*made;

	premade = ft_strjoin(one_path, "/");
	if (!premade)
		return (NULL);
	made = ft_strjoin(premade, cmd);
	if (!made)
	{
		free(premade);
		return (NULL);
	}
	free(premade);
	return (made);
}

static char	*combining(char **cmd_split, t_pipex *p)
{
	char	**path;
	int		i;
	char	*res;

	path = path_from_envp(p);
	if (!path)
		return (NULL);
	i = -1;
	while (path[++i])
	{
		res = make_full_path(path[i], cmd_split[0]);
		if (!res)
		{
			clean_arr(&path);
			error_clean_exit_code(ERR_MALLOC, EXIT_FAILURE, &p);
		}
		if (access(res, F_OK) == 0)
		{
			clean_arr(&path);
			return (res);
		}
		free(res);
	}
	clean_arr(&path);
	return (NULL);
}

char	*find_path(char **cmd_split, t_pipex *p)
{
	char	*combined_with_path;

	if (ft_strchr(cmd_split[0], '/') != NULL)
	{
		if (access(cmd_split[0], F_OK) == 0)
		{
			if (access(cmd_split[0], X_OK) == -1)
				cmd_error(PERM_DENIED, cmd_split[0], CMD_EXEC, &p);
			return (ft_strdup(cmd_split[0]));
		}
		else
			cmd_error(NO_FILE_DIR, cmd_split[0], CMD_NF, &p);
	}
	combined_with_path = combining(cmd_split, p);
	return (combined_with_path);
}
