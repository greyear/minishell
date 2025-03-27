/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 12:55:35 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 12:55:38 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Handles the execution of the `env` command, which prints the current 
 *        environment variables.
 * 
 * If the `env` command is called without arguments, it prints all environment 
 * variables stored in `ms->envp`. If any arguments are passed (other than 
 * `env`), an error message is printed and the exit status is set to 127.
 * 
 * @param args The arguments passed to the `env` command. The first argument 
 *             should be "env".
 * @param ms   A pointer to the `t_ms` structure containing environment 
 *             variables and exit status.
 */
void	handle_env(char	**args, t_ms *ms)
{
	ms->exit_status = 0;
	if (args[1])
	{
		print_env_error(args);
		ms->exit_status = 127;
		return ;
	}
	print_array(ms->envp);
}
