#include "../../include/minishell.h"

/**
 * @brief Processes each entry in the "unset" command and removes the corresponding
 *        environment variable from both the exported and shell environments.
 *
 * This function is called for each argument passed to the "unset" command. It 
 * extracts the environment variable name from the argument, checks if it's a valid 
 * key, and if valid, removes it from both the `exported` and `envp` lists. If the 
 * key is invalid, the function simply returns without making any changes.
 *
 * @param args The array of command arguments, with the environment variable to unset 
 *             located at the current index `i`.
 * @param i The index of the argument in `args` that specifies the environment 
 *          variable to unset.
 * @param ms The shell structure containing execution state information, including 
 *           the `exported` and `envp` environment lists.
 * @param len The length of the environment variable key to extract from the argument.
 */

static void	process_unset_entry(char **args, int i, t_ms *ms, int len)
{
	char	*name;

	name = extract_key(args[i], len);
	if (!name)
	{
		ms->exit_status = 1;
		return;
	}
	if (!check_if_valid_key(name))
	{
		free(name);
		return;
	}
	rm_from_env_ex(&ms->exported, name, len, 1);
	rm_from_env_ex(&ms->envp, name, len, 0);
	free(name);
}

/**
 * @brief Handles the "unset" command, which removes environment variables.
 *
 * This function processes the given arguments and attempts to remove the 
 * corresponding environment variables from both the exported and shell environments. 
 * If the argument is not a valid environment variable key, it skips the removal.
 * The exit status is updated based on the success or failure of the unset operation.
 *
 * @param args The array of command arguments.
 * @param ms The shell structure containing execution state information, including 
 *           the exit status.
 */

void	handle_unset(char **args, t_ms *ms)
{
	int		i;
	int		len;

	i = 1;
	ms->exit_status = 0;
	if (!args[1])
		return;
	if (args[1][0] && args[1][0] == '-')
	{
		print_flag_error(args);
		ms->exit_status = 2;
		return;
	}
	while (args[i])
	{
		len = get_key_length(args[i]);
		if (len != 0 || !(ft_strchr(args[i], '='))
			|| args[i][0] != '=')
			process_unset_entry(args, i, ms, len);
		i++;
	}
}