
#ifndef CONSTANTS_H
# define CONSTANTS_H

typedef enum e_bool
{
	false = 0,
	true = 1
}	t_bool;

typedef enum e_char
{
	NULL_TERM = 0,
	TAB = 9,
	NL = 10,
	WSPACE = 32,
	DB_QUOT = 34,
	DOLLAR = 36,
	AND = 38,
	SG_QUOT = 39,
	L_PARENT = 40,
	R_PARENT = 41,
	ASTER = 42,
	DASH = 45,
	DOT = 46,
	SLASH = 47,
	COLON = 58,
	SCOLON = 59,
	L_REDIR = 60,
	EQUAL = 61,
	R_REDIR = 62,
	QUEST = 63,
	BACKSLASH = 92,
	UNDERSC = 95,
	VERTICAL = 124
}	t_char;

typedef enum e_token_type
{
	DUMMY = 0, // empty first technical node
	PIPE = 1, // |
	IN = 2, // <
	OUT = 3, // >
	HEREDOC = 4, // <<
	APPEND = 5, // >>
	WORD = 6, // string
	SPACE = 7,
	END = 8, //?? mb ne nuzhen
	EMPTY = 9
}	t_token_type;

typedef enum e_err
{
	ARGS_NUM,
	INV_ARGS,
	MLLC,
	NO_FD = -1, //same as err output of open, dup2...
	DEF = -2, //divide into 2 structs? change values
}	t_err;

typedef enum e_print
{
	AMBIG,
	PERM_DEN,
	NO_FILE,
	DIRECT,
	HERED_ERR,
	HIST_ERR,
	NO_FILE_OR_DIR,
	IS_DIR,
	NO_CMD,

}	t_print;

typedef enum e_exit
{
	SUCCESS = 0,
	ERROR = 1,
	SYNTAX_ERR = 2,
	CMD_EXEC = 126,
	CMD_NF = 127,
	SIGNAL_HEREDOC = -3,
	MALLOC_ERR,
}	t_exit;

typedef enum e_oper
{
	RD,
	WR,
}	t_oper;

typedef enum e_mode
{
	DEFAULT,
	INTERACTIVE,
	HEREDOC_MODE,
	IGNORE,
}	t_mode;

/*typedef enum e_cmd_err
{
	PERM_DENIED,
	NO_FILE_OR_DIR,
	IS_DIR,
	NO_CMD,
}	t_cmd_err;*/

# define OWN_ERR_MSG "ms error: "
//# define PERM_DEN 1
//# define NO_FILE_OR_DIR 2
//# define IS_DIR 3
//# define NO_CMD 4
# define HISTORY_FILE ".history.tmp"
# define HISTORY_SIZE 500

/*POSIX ограничивает диапазон exit status от 0 до 255. 
При использовании значений выше 255 операционная система может 
"обрезать" коды до младших 8 бит, что вызовет некорректные 
результаты*/

#endif
