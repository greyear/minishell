/*
 * STILL MODIFYNG; THAT WHY SO LONGGGG*
 * *STILL TESTING***
	Handles cd
	cd = goes to home
	cd .. = goes to one directory up
	cd - = goes to the oldpwd directory so the one where we were before
	cd dir = goes to dir if it exists

	handle_cd(t_ms *ms, char **args)
	takes the struct as argument and
	args that are for example
	args[0]="cd", args[1]="-", args[2]=NULL

	SOME CASES
	if export HOME=hey and cd
	bash: cd: hey: No such file or directory
	if export HOME= and cd
	does nothing
	if unset HOME and cd
	bash: cd: HOME not set
	if export OLDPWD= and cd -
	print newline and then change OLDPWD value to same as PWD’s value
	if export OLDPWD=, export PWD=, and cd ..
	print newline and change PWD value to the actual directory path we are in
	if cd .. is used again, prints a newline, now when PWD has a value it changes OLDPWD value to same as PWD’s value

*/

#include "../../include/minishell.h"

static char	*get_cd_target(t_ms *ms, char **args)
{
	char	cwd[1024];

	if (!args[1])
		return (get_home_directory(ms, 0));
	if (ft_strcmp(args[1], "~") == 0)
		return (get_home_directory(ms, 1));
	if (ft_strcmp(args[1], "-") == 0)
		return (get_oldpwd_directory(ms));
	if (ft_strcmp(args[1], "..") == 0)
		return (get_parent_directory(ms));
	if (args[1][0] != '/')
	{
		if (getcwd(cwd, sizeof(cwd)) == NULL)
			return (NULL);
		return (build_relative_path(args[1], cwd));
	}
	return (ft_strdup(args[1]));
}

static void	update_cd_env(t_ms *ms, char *pwd_before)
{
	char cwd[1024];
	char	*current_pwd;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		printf("cd: getcwd failed");
		ms->exit_status = 1;
		return;
	}
	// fetch the current pwd value to the oldpwd's value
	// if PWD is unset, then use the actual drectory we were in (pwd_now), before we changed the
	// directory with chdir n prevous function
	current_pwd = get_env_value("PWD", ms->envp);
	if (!current_pwd)
		current_pwd = "";
	if (current_pwd && *current_pwd != '\0')
		update_env_var(ms, "OLDPWD=", current_pwd);
	else
		update_env_var(ms, "OLDPWD=", pwd_before);
	// If PWD is empty or not set, consider current directory as the new PWD
	update_env_var(ms, "PWD=", cwd);
}

void	update_env_var(t_ms *ms, char *key, char *new_value)
{
	int		i;
	int		len;
	char	*new_env_entry;

	i = 0;
	len = ft_strlen(key);
	new_env_entry = malloc(len + ft_strlen(new_value) + 1);
	if (!new_env_entry)
		return;
	ft_strcpy(new_env_entry, key);
	ft_strcat(new_env_entry, new_value);
	while (ms->envp[i])
	{
		if (ft_strncmp(ms->envp[i], key, len) == 0)
		{
			free(ms->envp[i]);
			ms->envp[i] = new_env_entry;
			return;
		}
		i++;
	}
}

static int	cd_error(t_ms *ms, char **args)
{
	if (!args || !*args)
		return (1);
	if (ft_strcmp(args[0], "cd") != 0)
		return (1);
	if (args[2])
	{
		ft_putstr_fd("bash: cd: too many arguments\n", 2);
		ms->exit_status = 1;
		return (1);
	}
	return (0);
}


void	handle_cd(t_ms *ms, char **args)
{
	char	*target_dir;
	char	cwd[1024];

	ms->exit_status = 0;
	if (cd_error(ms, args))
		return;
	target_dir = get_cd_target(ms, args);
	if (ms->exit_status == 1 || !target_dir)
		return;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		free(target_dir);
		ms->exit_status = 1;
		return;
	}
	if (chdir(target_dir) == -1)
	{
		print_cd_error(target_dir);
		free(target_dir);
		ms->exit_status = 1;
		return;
	}
	free(target_dir);
	update_cd_env(ms, cwd);
}
