#include "../../include/minishell.h"

static void clean_up_and_exit(int exit_status, t_ms *ms)
{
    clean_cmd_list(&(ms->cmds));
    history_exit(ms);
    clean_struct(ms);
    exit(exit_status);
}

static void handle_exit_code(long long exit_nbr, int error, char **array, t_ms *ms)
{
    if (error == 1 && ms)
    {
        print_numeric_error(array);
        clean_up_and_exit(2, ms);
    }
    if (exit_nbr > 255 || exit_nbr < 0)
        clean_up_and_exit(exit_nbr % 256, ms);
    clean_up_and_exit(exit_nbr, ms);
}

static void check_numeric_argument(char *arg, char **array, t_ms *ms)
{
    int		i;
	
	i = 0;
    if (arg[i] == '+' || arg[i] == '-')
        i++;
    while (arg[i])
    {
        if (!ft_isdigit(arg[i]))
            handle_exit_code(2, 1, array, ms);
        i++;
    }
}

static void handle_exit_argument(char **array, t_ms *ms)
{
    long long exit_nbr;
    int		error;

	error = 0;
    if (array[1])
    {
		check_numeric_argument(array[1], array, ms);
        if (array[2])
        {
            print_too_many_args_error();
            ms->exit_status = 1;
            return;
        }
        exit_nbr = ft_strtoll(array[1], &error);
        handle_exit_code(exit_nbr, error, array, ms);
    }
    clean_up_and_exit(ms->exit_status, ms);
}

void check_exit(char **array, t_ms *ms)
{
    if (!array || !*array)
        return;
    if (ft_strcmp(array[0], "exit") != 0)
        return;
    if (isatty(STDIN_FILENO))
        ft_putstr_fd("exit\n", STDOUT_FILENO);
    handle_exit_argument(array, ms);
}