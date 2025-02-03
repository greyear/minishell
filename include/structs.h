
#ifndef STRUCTS_H
# define STRUCTS_H

typedef struct s_token
{
	t_token_type	type;
	char			*data;
	char			*file;
	struct s_token	*next;
	t_char			quote;
	t_bool			unclosed;
	t_token_type	specific_redir;
}	t_token;


#endif