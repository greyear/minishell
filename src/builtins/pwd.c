/*
prints the current working directory.
Handles the pwd command by printing the current directory.
Uses getcwd() to get the actual path and displays it.
Prints an error if getcwd() fails.
prnts it out even if pwd 1 etc. many args, bs des this in bash 
*/


#include "../../include/minishell.h"

char	*get_env_value(char *key, char **envp)
{
    int		i;
    size_t	len;

	i = 0;
	len = ft_strlen(key);
    while (envp[i])
    {
        if (ft_strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
            return (envp[i] + len + 1); // Return value after "KEY="
        i++;
    }
    return (NULL); // Key not found
}

void	check_pwd(char **array, t_ms *ms)
{
	char	cwd[1024];

	if (!array || !*array)
		return;
	if (ft_strcmp(array[0], "pwd") != 0)
		return;
	ms->exit_status = 0;
	// Use getcwd() to get the actual directory
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
	{
		ms->exit_status = 1;
		perror("pwd: getcwd failed");
	}
}
