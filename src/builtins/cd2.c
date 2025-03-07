
#include "../../include/minishell.h"

char	*get_home_directory(t_ms *ms, int flag)
{
	char *temp;

	temp = get_env_value("HOME", ms->envp);
	if (!temp)
	{
		if (flag == 1)
			return (ft_strdup(getenv("HOME")));
		ft_putstr_fd("bash: cd: HOME not set\n", STDERR_FILENO);
        ms->exit_status = 1;
        return (NULL);
	}
	if (*temp == '\0')
	{
		ms->exit_status = 1;
		return (NULL);
	}
	if (access(temp, F_OK) != 0)
	{
		print_cd_error(temp);
		ms->exit_status = 1;
		return (NULL);
	}
	return (ft_strdup(temp));
}

char	*return_target(t_ms *ms, char *target)
{
    if (access(target, F_OK) != 0)
	{
        print_cd_error(target);
        ms->exit_status = 1;
        return (NULL);
    }
    ft_putendl_fd(target, STDOUT_FILENO);
	return (ft_strdup(target));
}

char	*get_parent_directory(t_ms *ms)
{
	char cwd[1024];
	char *parent_dir;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("cd: getcwd failed");
		ms->exit_status = 1;
		return (NULL);
	}
	parent_dir = ft_strrchr(cwd, '/');
	if (parent_dir && parent_dir != cwd)
	{
		*parent_dir = '\0';
		return (ft_strdup(cwd));
	}
	return (ft_strdup("/"));
}

char	*build_relative_path(char *target, char *cwd)
{
	char *temp;
	char *full_path;

	temp = ft_strjoin(cwd, "/");
	if (!temp)
		return (NULL);
	full_path = ft_strjoin(temp, target);
	free(temp);
	return (full_path);
}