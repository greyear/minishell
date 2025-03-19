#include "../../include/minishell.h"

/**
 * @brief Allocates memory for a new `t_ms` structure and initializes it.
 * 
 * This function allocates memory for a `t_ms` structure and initializes the 
 * `exit_status` field to 0. If memory allocation fails, it prints an error 
 * message and exits the program.
 * 
 * @return A pointer to the newly allocated `t_ms` structure.
 */

static t_ms	*allocate_struct(void)
{
	t_ms	*ms;

	ms = malloc(sizeof(t_ms));
	if (!ms)
	{
		perror("t_ms: memory allocation failed");
		exit(1);
	}
	ms->exit_status = 0;
	return (ms);
}

/**
 * @brief Initializes the `envp` variable in the shell without using the `envp` environment.
 * 
 * This function initializes the `envp` array in the `t_ms` structure, which is an array
 * of strings representing the environment variables. It copies the second and third 
 * elements from the `exported` array (which should contain exported variables) into `envp`.
 * If memory allocation fails during this process, the function prints an error message,
 *  frees the struct and exits with 1.
 *
 * The `envp` array will contain:
 * - `envp[0]` will be a copy of `exported[1]` (the second exported variable).
 * - `envp[1]` will be a copy of `exported[2]` (the third exported variable).
 * - `envp[2]` will be set to `NULL`, indicating the end of the array.
 * 
 * @param ms The main shell structure that contains `envp` and `exported`.
 */

static void	initialize_envp_without_envp(t_ms *ms)
{
	ms->envp = malloc(sizeof(char *) * 4);
	if (!ms->envp)
	{
		print_malloc_error();
		clean_struct(ms);
		exit(1);
	}
	ms->envp[0] = ft_strdup(ms->exported[1]);
	if (!ms->envp[0])
	{
		print_malloc_error();
		clean_struct(ms);
		exit(1);
	}
	ms->envp[1] = ft_strdup(ms->exported[2]);
	if (!ms->envp[1])
	{
		print_malloc_error();
		clean_struct(ms);
		exit(1);
	}
	ms->envp[2] = NULL;
}

static void	initialize_envp_without_envp2(t_ms *ms)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("pwd: getcwd failed");
		clean_struct(ms);
		exit(1);
	}
	ms->exported[1] = ft_strjoin("PWD=", cwd);
	if (!ms->exported[1])
	{
		print_malloc_error();
		clean_struct(ms);
		exit(1);
	}
	ms->exported[2] = ft_strdup("SHLVL=0");
	if (!ms->exported[2])
	{
		print_malloc_error();
		clean_struct(ms);
		exit(1);
	}
	ms->exported[3] = NULL;
	initialize_envp_without_envp(ms);
}

static void	initialize_without_envp(t_ms *ms)
{
	ms->exported = malloc(sizeof(char *) * 4);
	if (!ms->exported)
	{
		print_malloc_error();
		clean_struct(ms);
		exit(1);
	}
	ms->exported[0] = ft_strdup("OLDPWD");
	if (!ms->exported[0])
	{
		print_malloc_error();
		clean_struct(ms);
		exit(1);
	}
	initialize_envp_without_envp2(ms);
}

/**
 * @brief Initializes the `envp` and `exported` environment variables in the shell.
 * 
 * This function is responsible for initializing both the `envp` and `exported` arrays
 * in the `t_ms` structure, which represent the environment variables. It first checks if
 * the `envp` passed as an argument is valid. If `envp` is `NULL` or points to an empty array,
 * it calls `initialize_without_envp` to handle the case where no environment variables are provided.
 * Otherwise, it copies the values from the provided `envp` into both `envp` and `exported` arrays.
 * If memory allocation fails for either array, the function prints a memory allocation error, 
 * cleans up the `t_ms` structure, and exits the program.
 * 
 * The `envp` and `exported` arrays are copied from the provided `envp` argument, which contains 
 * the environment variables passed to the program at launch.
 * 
 * @param ms The main shell structure where `envp` and `exported` are stored.
 * @param envp The environment variables passed to the shell (typically from `main`).
 */

static void	initialize_envp(t_ms *ms, char **envp)
{
	if (!envp || !*envp)
	{
		initialize_without_envp(ms);
		ms->no_env = true;
		return;
	}
	ms->no_env = false;
	ms->envp = copy_map(envp);
	if (!ms->envp)
	{
		print_malloc_error();
		clean_struct(ms);
		exit(1);
	}
	ms->exported = copy_map(envp);
	if (!ms->exported)
	{
		print_malloc_error();
		clean_struct(ms);
		exit(1);
	}
}

static void	initialize_history(t_ms *ms)
{
	default_history(ms->history);
	ms->history_num = 0;
	if (open_read_history_file(ms) == 0)
		ms->history_file = true;
	else
	{
		print_system_error(HIST_ERR);
		ms->history_file = false;
	}
}

/**
 * @brief Initializes and allocates memory for the main shell structure.
 * 
 * This function:
 * - Allocates memory for a `t_ms` structure.
 * - Initializes environment variables (`envp`).
 * - Sets up tokens and command blocks.
 * - Initializes command history.
 * - Prepares file descriptors and heredoc-related data.
 * - Updates SHLVL in environmental variables if needed.
 * - Updates SHLVL in environmental variables if needed.
 * 
 * @param envp The environment variables inherited from the parent process.
 * 
 * @return A pointer to the initialized `t_ms` structure, or `NULL` if allocation fails.
 * 
 */
t_ms	*initialize_struct(char **envp)
{
	t_ms *ms;

	ms = allocate_struct();
	if (!ms)
		return (NULL);
	initialize_envp(ms, envp);
	ms->tokens = NULL;
	ms->blocks = NULL;
	initialize_history(ms);
	ms->heredoc_count = 0;
	ms->heredoc_files = NULL;
	check_shlvl(ms);
	g_sgnl = 0;
	return (ms);
}