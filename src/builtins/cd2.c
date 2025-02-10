#include "seela.h"

char	*get_home_directory(t_ms *ms)
{
	char *temp;

	temp = get_env_value("HOME", ms->envp);
	if (!temp)
	{
		ft_putstr_fd("bash: cd: HOME not set\n", 2);
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

static char	*return_target(t_ms *ms, char *target)
{
	 // Check if OLDPWD is a valid path before printing it
    if (access(target, F_OK) != 0)
	{
        print_cd_error(target);
        ms->exit_status = 1;
        return (NULL);
    }
    ft_putendl_fd(target, 1);
	return (ft_strdup(target));
}

char	*get_oldpwd_directory(t_ms *ms)
{
	char	*target;
	char	*current_pwd;
	char	cwd[1024];

	target = get_env_value("OLDPWD", ms->envp);
	if (!target || *target == '\0')
    {
        ft_putstr_fd("\n", 1);  // Print an empty line when OLDPWD is not set
        current_pwd = get_env_value("PWD", ms->envp);
        if (!current_pwd || *current_pwd == '\0')
        {
            if (getcwd(cwd, sizeof(cwd)))
                update_env_var(ms, "PWD=", cwd);  // Make sure PWD is updated
            return (NULL);
		}
        update_env_var(ms, "OLDPWD=", current_pwd);  // Set OLDPWD to PWD's value
        target = ft_strdup(current_pwd);
        if (!target)
        {
            ms->exit_status = 1;
            return (NULL);
        }
        return (target);
    }
	return (return_target(ms, target));
}

char	*get_parent_directory(void)
{
	char cwd[1024];
	char *parent_dir;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("cd: getcwd failed");
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
