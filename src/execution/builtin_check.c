
#include "../../include/minishell.h"

/**
 * @brief Checks if a given command is a shell built-in command.
 *        The function compares the command name with a predefined list of built-ins.
 * 
 * @param cmd A pointer to a `t_cmd` structure containing the command information.
 *            The `name` field should contain the command string to check.
 * 
 * @return Returns `1` if the command is a built-in (`echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`), 
 *         otherwise returns `0`. If `cmd->name` is NULL, returns `0`.
 */
int	is_builtin(t_cmd *cmd)
{
	if (!cmd->name)
	return (0);

	if ((ft_strcmp(cmd->name, "echo") == 0) || \
		(ft_strcmp(cmd->name, "cd") == 0) || \
		(ft_strcmp(cmd->name, "pwd") == 0) || \
		(ft_strcmp(cmd->name, "export") == 0) || \
		(ft_strcmp(cmd->name, "unset") == 0) || \
		(ft_strcmp(cmd->name, "env") == 0) || \
		(ft_strcmp(cmd->name, "exit") == 0))
		return (1);
	return (0);
}

/**
 * @brief Executes the corresponding function for a built-in command.
 *        The function determines which built-in command is being called and
 *        invokes the appropriate handler.
 * 
 * @param cmd A pointer to a `t_cmd` structure containing the command details.
 *            The `name` field should contain the command string, and `args`
 *            should hold the arguments.
 * @param ms A pointer to a `t_ms` structure representing the shell state.
 *           This is used to manage environment variables and execution context.
 * @param in_child An integer flag indicating whether the command is executed
 *                 in a child process (`1`) or in a parent process (`0`).
 *                 Some commands (`export` and `unset`) behave differently based
 *                 on this flag.
 */
void	handle_builtin(t_cmd *cmd, t_ms *ms, int in_child)
{
	if (ft_strcmp(cmd->name, "echo") == 0)
		handle_echo(cmd->args, ms);
	else if (ft_strcmp(cmd->name, "cd") == 0)
		handle_cd(cmd->args, ms);
	else if (ft_strcmp(cmd->name, "pwd") == 0)
		check_pwd(cmd->args, ms);
	else if (ft_strcmp(cmd->name, "export") == 0)
	{
		if (in_child && cmd->args[1])
			handle_export(cmd->args, NULL);
		else
			handle_export(cmd->args, ms);
	}
	else if (ft_strcmp(cmd->name, "unset") == 0)
	{
		if (!in_child)
			handle_unset(cmd->args, ms);
	}
	else if (ft_strcmp(cmd->name, "env") == 0)
		handle_env(cmd->args, ms);
	else if (ft_strcmp(cmd->name, "exit") == 0)
		check_exit(cmd->args, ms);
}

/**
 * @brief Determines whether a built-in command should be executed in a child process.
 * 
 * Some built-in commands, such as `cd`, `exit`, `unset`, and `export` (with arguments),
 * should be executed in the main process.
 * This function checks the command type and decides if a child process is required.
 * 
 * @param cmd A pointer to the `t_cmd` structure containing the command details.
 *            The `name` field specifies the command, and `args` holds its arguments.
 * 
 * @return `true` if the command should be executed in a child process,
 *         `false` if it should be executed in the main shell process.
 */
int	if_children_needed(t_cmd *cmd)
{
	if (ft_strcmp(cmd->name, "cd") == 0 || \
		ft_strcmp(cmd->name, "exit") == 0 || \
		ft_strcmp(cmd->name, "unset") == 0 || \
		(ft_strcmp(cmd->name, "export") == 0 && cmd->args[1]))
		return (false);
	return (true);
}