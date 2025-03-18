
#ifndef STRUCTS_H
# define STRUCTS_H

# include <sys/types.h> 

typedef struct s_token
{
	//index?
	t_token_type	type;
	char			*data;
	char			*file;
	t_char			quote;
	t_bool			unclosed;
	t_token_type	specific_redir;
	t_bool			ambiguous;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	char			*name;
	char			**args;
	int				num;
	int				infile;
	int				outfile;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_block
{
	t_token			*start;
	t_token			*end;
	struct s_block	*next;
}	t_block;

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
	char	**heredoc_files; // Store heredoc filenames
	int		heredoc_count;   // Number of heredoc files
} t_ms;

typedef struct s_envp
{
	char			*name;
	char			*value;
	struct s_envp	*next;
}	t_envp;

typedef struct s_pipe
{
	int		num_cmds;
	int		cmd_num;
	int		fd[2];
	int		cur_fd;
	pid_t	last_pid;
	pid_t	*pids;
	t_ms	*ms;
}	t_pipe;

/*typedef struct s_terminal
{
	struct termios	new;
	struct termios	old;
}	t_terminal;*/

#endif