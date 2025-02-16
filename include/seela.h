#ifndef SEELA_H
# define SEELA_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <stdio.h>
# include "../libft/include/libft.h"
# include <errno.h>

typedef struct s_ms
{
    int     exit_status; // Store the last exit status
    char    **envp;     // Environment variables
    char    **exported;
} t_ms;

//env_export2.c
void    handle_env(t_ms *ms); //ENV HANDLING
void    print_exported(t_ms *ms);
void    sort_exported_alphaorder(t_ms *ms);
void	print_error3(t_ms *ms, char *arg);

//env_export.c
void    handle_export(char **args, t_ms *ms); //EXPORT HANDLING
void    add_to_exported_env(char *arg, t_ms *ms);
void    add_to_env(char *arg, t_ms *ms, char *name, int len);
void    add_to_exported(char *arg, t_ms *ms, char *name, int len);
void    update_exported(char *arg, t_ms *ms);
void	update_env_var(t_ms *ms, char *key, char *new_value);

//unset.c
void	handle_unset(char **args, t_ms *ms); //UNSET HANDLING
//static void	rm_from_env(t_ms *ms, char *name, int len);
//static void	rm_from_export(t_ms *ms, char *name, int len);

//double_array_handling.c
char	**copy_map(char **original_map);
void	ft_command(char **envp, char *cmd);
void    print_array(char **m);

//cd.c
void	handle_cd(t_ms *ms, char **args); //CD HANDLING

//cd2.c
char	*build_relative_path(char *target, char *cwd);
char	*get_parent_directory(t_ms *ms);
char	*get_oldpwd_directory(t_ms *ms);
char	*get_home_directory(t_ms *ms, int flag);

//cd_error.c
void	print_cd_error(char *target_dir);

//pwd.c
void	check_pwd(t_ms *ms, char **array);
char	*get_env_value(char *key, char **envp);

//exit.c
void	check_exit(char	**array, t_ms *ms); //HANDLES EXITING
void	free_struct(t_ms *ms); //FREES STRUCT THAT WAS INITIALIZED AND MALLOCED

//exit2.c
long long	ft_strtoll(char *str, int *error);

//handle_expansion.c
char	*handle_expansion(char *args, t_ms *ms); //ENVIRONMENTAL EXPANSION HANDLNG
char	*expand_key(char **envp, char *key, int len, t_ms *ms);

//initialize_struct.c
//void    initialize_struct(char **envp); how do you use it?
t_ms    *initialize_struct(char **envp);

//cmd_errormsg_free.c  (FOR PIPE AND EXECUTION ERRORS)
void	ft_print_err(char *cmd, int c);
void	ft_free_array(char **array);
void    print_array(char **m);

//find_path_and_execute_command.c
void	ft_command(char **envp, char *cmd);

//pipe.c
void    execute_cmd(int num_cmds, char **argv, t_ms *ms); //PIPE AND EXECVE HANDLING

//redrections.c
void	redirection_outfile_emptied(char *file); //< HANDLING
void	redirection_infile(char *file); //> HANDLING
void	redirection_outfile_append(char *file); //<< HANDLING
void    handle_heredoc(char *limiter); //>> HANDLNG

#endif
