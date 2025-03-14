
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
# include <sys/stat.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <dirent.h> // Needed for opendir()
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <signal.h>
//# include <bits/types.h>
# include <sys/ioctl.h>
# include <termios.h>

extern volatile sig_atomic_t	g_sgnl;


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
long long	    convert_to_ll(char *str, int *error);
//char	        *handle_expansion(char *args, t_ms *ms);
char	        *handle_expansion(char *args, t_ms *ms, t_char quote);
char	        *expand_key(char **envp, char *key, int len, t_ms *ms);
char	        *duplicate_or_replace(char *entry, char *arg, char *key, int *flag);
char	        *return_target(t_ms *ms, char *target);

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
void            make_one_child(t_cmd *cmd, t_ms *ms);
void	        execute_command(char **envp, char **cmd);
void	        make_multiple_childs(int num_cmds, t_cmd *cmds, t_ms *ms);
void            pipe_process(int prev_pipe, int next_pipe);
void            redirect_process(int infile, int outfile);
void            pipe_or_redir(t_cmd *cur, int *pipe_fd, int i, int num_cmds, int cur_fd);
void	        handle_absolute_or_relative_path(char **envp, char **cmds);
void            handle_no_path_variable(char **envp, char **cmd);
void            check_if_dot(char **cmds);
void            setup_pipes(int *pipe_fd, int i, int num_cmds, int cur_fd);


//Envp
int				check_list_for_expansions(t_token *first, t_ms *ms);
int				expand_in_token(t_token *cur, t_ms *ms);
//void			expand_variable(t_ms *ms, char *key, int key_len, char **result);
void			expand_variable(t_ms *ms, char *key, int key_len, char **result, t_char quote);
t_envp			*envp_from_list(t_envp *list, char *name);
t_bool			is_envp_symbol(int c);
t_bool			is_envp_first_symbol(int c);
t_ms			*initialize_struct(char **envp);
void			check_shlvl(t_ms *ms);
char			*remove_extra_spaces(char *str);

//Redirections
void			put_infile_fd(t_token *token, t_cmd *cmd);
void			put_outfile_fd(t_token *token, t_cmd *cmd);
void			check_access(char *filename, t_oper operation);
char            *generate_heredoc_filename(int index);
void	        put_heredoc_fd(t_token *token, t_cmd *cmd, t_ms *ms);
int             handle_heredoc(t_ms *ms, char *limiter, t_token *token);
void	        reset_heredocs(t_ms *ms);

//Reading + history
t_bool			open_read_history_file(t_ms *ms);
void			write_to_history_file(t_ms *ms);
void			add_line_to_history(char *line, t_ms *ms);
void			default_history(char **history);
void			fill_ms_history_line(char *line, t_ms *ms);
void			clean_ms_history(t_ms *ms);
void			history_exit(t_ms *ms);

//Signals
void			signal_mode(t_mode mode);

//Errors
void			print_file_error(char *file, t_print reason);
void			print_system_error(t_print reason);
void			print_cmd_error(char *cmd, int c);
void			print_unset_error(char **args, int i, t_ms *ms);
void			print_export_error(t_ms *ms, char *arg);
void			print_cd_error(char *target_dir);
void			print_syntax_error(char *text);
void	        print_numeric_error(char **array);
void	        print_too_many_args_error(void);
void	        print_env_error(char **args);
void	        print_flag_error(char **args);

//Cleaners
void			clean_arr(char ***arr);
void			free_int_array(int **array);
void			clean_struct(t_ms *ms);
void			clean_struct_fields(t_ms *ms);
void			cleanup_heredocs(char **filenames);
void            close_pipes(int num_cmds, int **pipe_fd);
void	        free_pids(t_pipe *p);
void	        close_fds(t_cmd *cmd);
void            close_all_fds(t_pipe *p, t_ms *ms);
void            close_file(int file);
void            close_fds2(int fd1, int fd2);

#endif