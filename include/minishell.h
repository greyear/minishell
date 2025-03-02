
#ifndef MINISHELL_H
# define MINISHELL_H

# include "constants.h"
# include "structs.h"
# include "../libft/include/libft.h"
# include "../libft/include/ft_printf.h"
# include "../libft/include/get_next_line.h"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <dirent.h> // Needed for opendir()
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>

# define ERR_MALLOC "memory allocation failed"
# define ERR_FORK "fork function failed"
# define ERR_PIPE "pipe creation failed"
# define ERR_DUP2 "dup2 function failed"
# define ERR_OPEN "opening failed"
# define ERR_CLOSE "closing failed"
# define NO_FILE_DIR "no such file or directory"
# define ZSH_NO_FILE "zsh: no such file or directory:"
# define CMD_NOT_FOUND "zsh: command not found:"
# define PERM_DENIED "zsh: permission denied:"

# define CMD_EXEC 126
# define CMD_NF 127

//BNF
char			*validate_word(char *str, int *err_flag);
char			*validate_redirect(char *str, int *err_flag);
char			*validate_simple_cmd(char *str, int *err_flag);
char			*validate_cmd(char *str, int *err_flag);
char			*validate_pipeline(char *str, int *err_flag);
int				validate_input(char *str);
int				ft_special(int c);

//BUILTINS
void	        handle_echo(char **args, t_ms *ms);
void            handle_env(char **args, t_ms *ms);
void            print_exported(t_ms *ms);
void        	add_to_exported(char *key, t_ms *ms);
void            sort_exported_alphaorder(t_ms *ms);
void            handle_export(char **args, t_ms *ms);
void            handle_unset(char **args, t_ms *ms);
void	        rm_from_env_ex(char ***env, char *name, int len, int flag);
int             check_env(char *env, char *name, int len, int flag);
char	        **allocate_temp_env(char **env, int x);
char	        **copy_map(char **original_map);
void            print_array(char **a);
int		        check_if_valid_key(char *name); 
int		        get_key_length(char *arg);
char	        *extract_key(char *arg, int len);
void	        handle_cd(char **args, t_ms *ms);
void	        update_env_var(t_ms *ms, char *key, char *new_value);
char	        *build_relative_path(char *target, char *cwd);
char	        *get_parent_directory(t_ms *ms);
char	        *get_oldpwd_directory(t_ms *ms);
char	        *get_home_directory(t_ms *ms, int flag);
void	        check_pwd(char **array, t_ms *ms);
char	        *get_env_value(char *key, char **envp);
void	        check_exit(char	**array, t_ms *ms);
long long	    ft_strtoll(char *str, int *error);
char	        *handle_expansion(char *args, t_ms *ms);
char	        *expand_key(char **envp, char *key, int len, t_ms *ms);
void	        ft_command(char **envp, char **cmd);
void	        execute_cmds(int num_cmds, t_cmd *cmds, t_ms *ms);

//Lexer
t_token			*tokenization(char *str, t_ms *ms);
t_token_type	define_token_type(char *str, size_t i);
t_token			*create_new_token(char *str, size_t *i, t_token_type type);
char			*word_with_quotes(char *str, size_t *start, t_token * new);
char			*word_without_quotes(char *str, size_t *start);
void			skip_special_tokens(char *str, size_t *i, t_token_type type);
void			skip_whitespaces(char *str, size_t *i);
void			clean_token(t_token *token);
t_bool			is_empty_word_token(t_token *token);
void			clean_token_list(t_token **first);
t_token			*delete_empty_word_tokens(t_token *first);
t_token			*delete_whitespace_tokens(t_token *first);
t_token			*unite_two_word_tokens(t_token *first);
t_bool			is_redirect(t_token_type type);
void			flags_for_redirections(t_token *cur);
void			put_files_for_redirections(t_token *cur);
void			print_tokens(t_token *token_list);
int				check_list_for_tilde(t_token *first, t_ms *ms);

//Parser
int				words_in_cmd_block(t_token *start, t_token *end);
int				put_cmg_args(t_cmd *cmd, t_token *start, t_token *end);
t_cmd			*create_new_cmd(t_block *block, int num, t_ms *ms);
t_cmd			*create_cmd_list(t_block *block, t_ms *ms);
t_cmd			*clean_cmd(t_cmd *cmd);
t_cmd			*clean_cmd_list(t_cmd **first);
int				check_block(t_token *start, t_token *end, int *err_flag);
t_block			*create_block(t_token *start, t_token *end, t_block *first_block, int *err_flag);
t_block			*create_blocks_list(t_token *start, t_token *end, int *err_flag);
void			redir_in_block(t_block *block, t_cmd *cmd, t_ms *ms);
t_block			*clean_block(t_block *block);
t_block			*clean_block_list(t_block **first);

//Execution
int				is_builtin(t_cmd *cmd);
void			handle_builtin(t_cmd *cmd, t_ms *ms, int in_child);
int				if_children_needed(t_cmd *cmd);
void            execute_single_cmd(t_cmd *cmd, t_ms *ms);

//Envp
int				check_list_for_expansions(t_token *first, t_ms *ms);
int				expand_in_token(t_token *cur, t_ms *ms);
t_envp			*envp_from_list(t_envp *list, char *name);
t_bool			is_envp_symbol(int c);
t_bool			is_envp_first_symbol(int c);
t_ms			*initialize_struct(char **envp);

//Redirections
void			put_infile_fd(t_token *token, t_cmd *cmd);
void			put_outfile_fd(t_token *token, t_cmd *cmd);
void			check_access(char *filename, t_oper operation);
char            *generate_heredoc_filename(int index);
void	        put_heredoc_fd(t_token *token, t_cmd *cmd, t_ms *ms);
int             handle_heredoc(t_ms *ms, char *limiter);

//Reading + history
t_bool			open_read_history_file(t_ms *ms);
void			write_to_history_file(t_ms *ms);
void			add_line_to_history(char *line, t_ms *ms);
void			default_history(char **history);
void			clean_ms_history(t_ms *ms);
void			history_exit(t_ms *ms);


/*
//Pipex
void	init_p(t_pipex	*p);
char	**path_from_envp(t_pipex *p);
char	*find_path(char **cmd_split, t_pipex *p);
char	**split_cmd(char *cmd, t_pipex *p);
void	handle_command(char *cmd, t_pipex *p);

//Utils
void	close_fds(int fd1, int fd2);
int		length_inside(char *str, t_pipex *p, char *cmd);
int		word_length(char *str);*/

//Errors
void			print_file_error(char *file, t_print reason);
void			print_system_error(t_print reason);
void            print_cmd_error(char *cmd, int c);
void            print_unset_error(char **args, int i, t_ms *ms);
void            print_export_error(t_ms *ms, char *arg);
void            print_cd_error(char *target_dir);
//void	args_number_error(void);
/*void	execve_fail(char **path, char **cmd_split, t_pipex **p);
void	cmd_error(char *reason, char *cmd, int exit_code, t_pipex **p);
void	error_exit_code(char *reason, int exit_code);
void	error_clean_exit_code(char *reason, int exit_code, t_pipex **p);*/

/*
//Checks
void	check_first_file(t_pipex *p);
void	check_second_file(t_pipex *p);*/

//Cleaners
void			clean_arr(char ***arr);
void            free_int_array(int **array);
void			clean_struct(t_ms *ms);
void			clean_struct_fields(t_ms *ms);
void            cleanup_heredocs(char **filenames);

#endif
