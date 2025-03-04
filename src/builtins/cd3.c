#include "../../include/minishell.h"

static char	*handle_missing_oldpwd(t_ms *ms)
{
	ft_putstr_fd(OWN_ERR_MSG, 2);
	ft_putstr_fd("cd: OLDPWD not set\n", 2);
	ms->exit_status = 1;
	return (NULL);
}

static char	*handle_empty_oldpwd(t_ms *ms)
{
	char	*current_pwd;
	char	cwd[1024];

	ft_putstr_fd("\n", STDOUT_FILENO);  // Print an empty line for an empty OLDPWD
	current_pwd = get_env_value("PWD", ms->envp);
	if (!current_pwd || *current_pwd == '\0')
	{
		if (getcwd(cwd, sizeof(cwd)))
			update_env_var(ms, "PWD=", cwd);  // Ensure PWD is updated
		return (NULL);
	}
	update_env_var(ms, "OLDPWD=", current_pwd);  // Set OLDPWD to PWD
	return (ft_strdup(current_pwd));
}

char	*get_oldpwd_directory(t_ms *ms)
{
	char	*target;

	target = get_env_value("OLDPWD", ms->envp);
	if (!target)
		return (handle_missing_oldpwd(ms));
	if (*target == '\0')
		return (handle_empty_oldpwd(ms));
	return (return_target(ms, target));
}

