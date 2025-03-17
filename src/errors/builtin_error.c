#include "../../include/minishell.h"

/**
 * @brief Prints an error message when an invalid identifier is passed to the `unset` command.
 * 
 * This function prints an error message to `stderr` when the user tries to unset an environment
 * variable or identifier that is not valid. The error message includes the argument that caused
 * the error.
 * 
 * @param args The array of arguments passed to the `unset` command.
 * @param i The index of the invalid argument in the `args` array.
 * @param ms The `t_ms` structure containing the exit status and other necessary data.
 * 
 * @return This function does not return a value. It sets the exit status to 1, indicating an error.
 */

 void	print_unset_error(char **args, int i, t_ms *ms)
 {
	 ft_putstr_fd(OWN_ERR_MSG, STDERR_FILENO);
	 ft_putstr_fd("unset: '", STDERR_FILENO);
	 ft_putstr_fd(args[i], STDERR_FILENO);
	 ft_putstr_fd("': not a valid indentifier\n", STDERR_FILENO);
	 ms->exit_status = 1;
 }
 
 /**
  * @brief Prints an error message indicating that flags are not supported.
  *
  * This function handles the printing of an error message when flags are passed as arguments in a command.
  * It outputs a custom error message to `stderr`, including the name of the command (from `args[0]`) and a specific
  * message indicating that flags are not supported in the shell.
  * 
  * @param args A pointer to the array of arguments, where `args[0]` is the command that invoked the error.
  *
  * @return None. The function does not return any value. It prints an error message to `stderr`.
  *
  */
 
 void	print_flag_error(char **args)
 {
	 ft_putstr_fd(OWN_ERR_MSG, STDERR_FILENO);
	 ft_putstr_fd(args[0], STDERR_FILENO);
	 ft_putstr_fd(": flags are not supported in minishell\n", STDERR_FILENO);
 }
 
 /**
  * @brief Prints an error message when an invalid identifier is passed to the `export` command.
  * 
  * This function prints an error message to `stderr` when the user tries to export an environment
  * variable or identifier that is not valid. The error message includes the argument that caused
  * the error.
  * 
  * @param ms The `t_ms` structure containing the exit status and other necessary data.
  * @param arg The argument passed to the `export` command that caused the error.
  * 
  * @return This function does not return a value. It sets the exit status to 1, indicating an error.
  */
 
 void	print_export_error(t_ms *ms, char *arg)
 {
	 ft_putstr_fd(OWN_ERR_MSG, STDERR_FILENO);
	 ft_putstr_fd("export: '", STDERR_FILENO);
	 ft_putstr_fd(arg, STDERR_FILENO);
	 ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	 ms->exit_status = 1;
 }
 
 /**
  * @brief Prints an error message when the `cd` command fails due to a non-existent directory.
  * 
  * This function prints an error message to `stderr` when the user attempts to change to a directory
  * that does not exist. If a target directory is provided, it will be displayed in the error message.
  * If no target directory is provided, a generic error message will be printed.
  * 
  * @param target_dir The directory that the user tried to change to. If `NULL` or an empty string,
  *        a generic error message is printed.
  * 
  * @return This function does not return a value. It directly prints an error message to `stderr`.
  */
 
 void	print_cd_error(char *target_dir)
 {
	 ft_putstr_fd(OWN_ERR_MSG, STDERR_FILENO);
	 ft_putstr_fd("cd: ", STDERR_FILENO);
	 if (target_dir && *target_dir)
		 ft_putstr_fd(target_dir, STDERR_FILENO);
	 else
		 ft_putstr_fd(" ", STDERR_FILENO);
	 ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
 }
 
 /**
  * @brief Prints an error message when the `env` command encounters a non-existent file or directory.
  * 
  * This function prints an error message to `stderr` when the `env` command is provided with an argument
  * that does not correspond to an existing file or directory. The error message includes the invalid argument
  * that was passed to the `env` command.
  * 
  * @param args The array of arguments passed to the `env` command. The function specifically uses `args[1]`
  *        (the first argument after the command itself) to display the invalid argument in the error message.
  * 
  * @return This function does not return a value. It directly prints an error message to `stderr`.
  */
 
 void	print_env_error(char **args)
 {
	 ft_putstr_fd(OWN_ERR_MSG, STDERR_FILENO);
	 ft_putstr_fd("env: '", STDERR_FILENO);
	 ft_putstr_fd(args[1], STDERR_FILENO);
	 ft_putstr_fd("': No such file or directory\n", STDERR_FILENO);
 }