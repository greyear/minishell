
#ifndef MINISHELL_H
# define MINISHELL_H

# include "constants.h"
# include "structs.h"
# include "../libft/include/libft.h"
# include "../libft/include/ft_printf.h"
# include "../libft/include/get_next_line.h"
# include "seela.h" //fix!
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>

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

typedef struct s_pipex
{
	int		argc;
	char	**argv;
	char	**envp;
	pid_t	*pids;
	char	**cmds;
	int		cmd_num;
	int		fd[2];
	int		cur_fd;
}	t_pipex;

//BNF
char			*validate_word(char *str, int *err_flag);
char			*validate_redirect(char *str, int *err_flag);
char			*validate_simple_cmd(char *str, int *err_flag);
char			*validate_cmd(char *str, int *err_flag);
char			*validate_pipeline(char *str, int *err_flag);
int				validate_input(char *str);
int				ft_special(int c);

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
t_block			*clean_block(t_block *block);
t_block			*clean_block_list(t_block **first);

//Envp
int				check_list_for_expansions(t_token *first, t_ms *ms);
int				expand_in_token(t_token *cur, t_ms *ms);
t_envp			*envp_from_list(t_envp *list, char *name);
t_bool			is_envp_symbol(int c);
t_bool			is_envp_first_symbol(int c);
t_ms			*initialize_struct(char **envp);

/*
//Pipex
void	init_p(t_pipex	*p);
char	**path_from_envp(t_pipex *p);
char	*find_path(char **cmd_split, t_pipex *p);
char	**split_cmd(char *cmd, t_pipex *p);
void	handle_command(char *cmd, t_pipex *p);
int		open_infile(t_pipex *p);
int		open_outfile(t_pipex *p);

//Utils
void	close_fds(int fd1, int fd2);
int		length_inside(char *str, t_pipex *p, char *cmd);
int		word_length(char *str);*/

//Errors
//void	args_number_error(void);
void	execve_fail(char **path, char **cmd_split, t_pipex **p);
void	cmd_error(char *reason, char *cmd, int exit_code, t_pipex **p);
void	error_exit_code(char *reason, int exit_code);
void	error_clean_exit_code(char *reason, int exit_code, t_pipex **p);

/*
//Checks
void	check_first_file(t_pipex *p);
void	check_second_file(t_pipex *p);*/

//Cleaners
void	clean_arr(char ***arr);
void	ft_free_map(char **map);

#endif
