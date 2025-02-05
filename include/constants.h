
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
	/*L_REDIR_SEPARATOR = 29,
	R_REDIR_SEPARATOR = 30,
	SEPARATOR = 31,*/
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
	//N_LOWER = 110,
	VERTICAL = 124
}	t_char;

typedef enum e_token_type
{
	PIPE, // |
	IN, // <
	OUT, // >
	HEREDOC, // <<
	APPEND, // >>
	WORD, // string
	LPAR, // (
	RPAR, // )
	SPACE,
	END, //?? mb ne nuzhen
	//just &?
	DUMMY, // for the first one
	EMPTY
}	t_token_type;

typedef enum e_err
{
	ARGS_NUM,
	INV_ARGS,
	MLLC
}	t_err;

typedef enum e_exit
{
	SUCCESS = 0,
	ERROR = 1,
	CMD_EXEC = 126,
	CMD_NF = 127,
	MALLOC_ERR,
}	t_exit;


/*POSIX ограничивает диапазон exit status от 0 до 255. 
При использовании значений выше 255 операционная система может 
"обрезать" коды до младших 8 бит, что вызовет некорректные 
результаты*/

#endif