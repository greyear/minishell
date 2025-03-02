#ifndef SEELA_H
# define SEELA_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <stdio.h>
# include "../libft/include/libft.h"
# include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_ms
{
	int		exit_status; // Store the last exit status
	char	**envp;     // Environment variables
	char	**exported; // store exported variables
	t_token	*tokens; //Tried to put this all here not to have problems with cleaning
	t_block	*blocks;
	t_cmd	*cmds;
	t_bool	history_file; //yes (1) if save, no (0) if can't save in file
	char	*history[500];
	int		history_num;
    char    **heredoc_files; // Store heredoc filenames
    int     heredoc_count;   // Number of heredoc files
} t_ms;

//echo.c
void	handle_echo(char **args, t_ms *ms); // ECHO HANDLING

//env.c
void    handle_env(char **args, t_ms *ms); //ENV HANDLING

//export3.c
void    print_exported(t_ms *ms);
void	print_error3(t_ms *ms, char *arg);

//export2.c
void	add_to_exported(char *key, t_ms *ms);
void    sort_exported_alphaorder(t_ms *ms);

//export.c
void    handle_export(char **args, t_ms *ms); //EXPORT HANDLING

//unset.c
void	handle_unset(char **args, t_ms *ms); //UNSET HANDLING

//unset2.c
void	rm_from_env_ex(char ***env, char *name, int len, int flag);

//helper_functions.c
int	check_env(char *env, char *name, int len, int flag);
char	**allocate_temp_env(char **env, int x);
char	**copy_map(char **original_map); //COPYING **ARRAYS
void    print_array(char **a); // PRINTING **ARRAYS

//key_handling.c
int		check_if_valid_key(char *name); 
int		get_key_length(char *arg);
char	*extract_key(char *arg, int len);

//cd.c
void	handle_cd(char **args, t_ms *ms); //CD HANDLING
void	update_env_var(t_ms *ms, char *key, char *new_value);

//cd2.c
char	*build_relative_path(char *target, char *cwd);
char	*get_parent_directory(t_ms *ms);
char	*get_oldpwd_directory(t_ms *ms);
char	*get_home_directory(t_ms *ms, int flag);

//pwd.c
void	check_pwd(char **array, t_ms *ms);
char	*get_env_value(char *key, char **envp);

//exit.c
void	check_exit(char	**array, t_ms *ms); //EXIT HANDLING
void	free_struct(t_ms *ms); //FREES STRUCT

//exit2.c
long long	ft_strtoll(char *str, int *error);

//expansion.c
char	*handle_expansion(char *args, t_ms *ms); //ENVIRONMENTAL EXPANSION HANDLNG
char	*expand_key(char **envp, char *key, int len, t_ms *ms);

//initialize_struct.c
t_ms    *initialize_struct(char **envp);
char	**make_args(char *cmd, char *name);

//find_path_and_execute_command.c
void	ft_mmand(char **envp, char **cmd);

//execute_cmd.c
void	execute_cmd(int num_cmds, t_cmd *cmds, t_ms *ms); //PIPE AND EXECVE HANDLING

//generate_filename.c
char *generate_heredoc_filename(int index);

//fds.c
void	put_heredoc_fd(t_token *token, t_cmd *cmd, t_ms *ms);

//heredoc.c
int    handle_heredoc(t_ms *ms, char *limiter); //>> HANDLNG
void    cleanup_heredocs(char **filenames);

#endif
