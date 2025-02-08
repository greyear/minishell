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
	if export OLDPWD=, export PWD=, and cd -
	print newline and change PWD value to the actual directory path we are in
	if cd - is used again, prints a newline, now when PWD has a value it changes OLDPWD value to same as PWD’s value

	cd .. still works even if PWD=invalidpath and OLDPWD=invalidpath

*/

#include "seela.h"

static void	print_cd_error(char *target_dir)
{
	ft_putstr_fd("bash: cd: ", 2);
	if (target_dir && *target_dir)
		ft_putstr_fd(target_dir, 2);
	else
		ft_putstr_fd(" ", 2);
	ft_putstr_fd(": No such file or directory\n", 2);
}

static void	add_to_env_end(t_ms *ms, char *new_env_entry, int i)
{
	int		j;
	char	**new_envp;

	j = 0;
	 // Key not found, need to expand envp
    new_envp = malloc((i + 2) * sizeof(char *)); // +2 for new var & NULL
    if (!new_envp)
    {
        free(new_env_entry);
        return;
    }
    // Copy old envp
    while (j < i)
	{
        new_envp[j] = ms->envp[j];
		j++;
	}
    new_envp[i] = new_env_entry;
    new_envp[i + 1] = NULL;

    free(ms->envp); // Free old envp array
    ms->envp = new_envp;
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

static char	*get_cd_target(t_ms *ms, char **args)
{
	char	*target;
	char	*parent_dir;
	char	*current_dir;
	char	cwd[1024]

	if (!args[1])
	{
		target = get_env_value("HOME", ms->envp);
		if (!target) //if no HOME
		{
			ft_putstr_fd("bash: cd: HOME not set\n", 2);
			ms->exit_status = 1;
			return (NULL);
		}
		if (*target == '\0') //if HOME=
		{
			ms->exit_status = 1;
			return (NULL);
		}
		if (access(target, F_OK) != 0)
		{
			print_cd_error(target);
			ms->exit_status = 1;
			return (NULL);
		}
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		target = get_env_value("OLDPWD", ms->envp);
		if (!target || *target == '\0')
		{
			ft_putstr_fd("\n", 1);
			current_pwd = get_env_value("PWD", ms->envp);
			if (!current_pwd || *current_pwd == '\0')
			{
				if (getcwd(cwd, sizeof(cwd)))
					update_env_var(ms, "PWD=", cwd);
				return (NULL);
			}
			update_env_var(ms, "OLDPWD=", current_pwd);
			return (current_pwd);
		}
		ft_putendl_fd(target, 1);
	}
	else if (ft_strcmp(args[1], "..") == 0) // Handle ..
    {
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("cd: getcwd failed");
            ms->exit_status = 1;
            return (NULL);
        }
        parent_dir = ft_strrchr(cwd, '/');
        if (parent_dir && parent_dir != cwd)
		{
            *parent_dir = '\0';  // Trim the last part of the path to go to the parent
            target = cwd;
        }
		else
            target = "/";
    }
	else // Handle other paths (either absolute or relative)
    {
        if (getcwd(cwd, sizeof(cwd)) == NULL)
        {
            perror("cd: getcwd failed");
            ms->exit_status = 1;
            return (NULL);
        }
		// If the target is relative, construct the full path
        if (args[1][0] != '/') // Check if it is relative
        {
            target = ft_strjoin(cwd, "/");
            target = ft_strjoin(target, args[1]);
        }
        else
        {
            // It's an absolute path
            target = args[1];
        }
    }
	return (target);
}

static void	update_cd_env(t_ms *ms)
{
	char	cwd[1024];

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

void	handle_cd(t_ms *ms, char **args)
{
	char	*target_dir;

	ms->exit_status = 0;
	if (ft_strcmp(args[0], "cd") != 0)
		return;
	target_dir = get_cd_target(ms, args);
	if (ms->exit_status == 1)
		return;
	if (chdir(target_dir) == -1)
	{
		print_cd_error(target_dir);
		ms->exit_status = 1;
		return;
	}
	update_cd_env(ms);
}
