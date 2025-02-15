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

#include "seela.h"

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

static void	update_cd_env(t_ms *ms)
{
	char cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		printf("cd: getcwd failed");
		ms->exit_status = 1;
		return;
	}
	update_env_var(ms, "OLDPWD=", get_env_value("PWD", ms->envp));
	update_env_var(ms, "PWD=", cwd);
	ms->exit_status = 0;
}

static void	add_to_env_end(t_ms *ms, char *new_env_entry, int i)
{
	int		j;
	char	**new_envp;

	j = 0;
	new_envp = malloc((i + 2) * sizeof(char *));
	if (!new_envp)
	{
		free(new_env_entry);
		return;
	}
	while (j < i)
	{
		new_envp[j] = ms->envp[j];
		j++;
	}
	new_envp[i] = new_env_entry;
	new_envp[i + 1] = NULL;
	ft_free_array(ms->envp);
	ms->envp = copy_map(new_envp);
	free(new_envp);
}

static void	update_env_var(t_ms *ms, char *key, char *new_value)
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
	add_to_env_end(ms, new_env_entry, i);
}

void	handle_cd(t_ms *ms, char **args)
{
	char	*target_dir;

	ms->exit_status = 0;
	if (ft_strcmp(args[0], "cd") != 0)
		return;
	target_dir = get_cd_target(ms, args);
	if (ms->exit_status == 1 || !target_dir)
		return;
	if (chdir(target_dir) == -1)
	{
		print_cd_error(target_dir);
		free(target_dir);
		ms->exit_status = 1;
		return;
	}
	free(target_dir);
	update_cd_env(ms);
}
