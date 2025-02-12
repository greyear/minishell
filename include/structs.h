
#ifndef STRUCTS_H
# define STRUCTS_H

typedef struct s_token
{
	//index?
	t_token_type	type;
	char			*data;
	char			*file;
	t_char			quote;
	t_bool			unclosed;
	t_token_type	specific_redir;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	char			*name;
	char			**args;
	int				num;
	struct s_cmd	*next;
	//mb smth else
}	t_cmd;

typedef struct s_block
{
	t_token			*start;
	t_token			*end;
	struct s_block	*next;
}	t_block;

typedef struct s_envp
{
	char			*name;
	char			*value;
	struct s_envp	*next;
}	t_envp;


#endif