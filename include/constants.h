/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssalorin <ssalorin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 12:42:52 by ssalorin          #+#    #+#             */
/*   Updated: 2025/03/27 12:42:55 by ssalorin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	DUMMY = 0,
	PIPE = 1,
	IN = 2,
	OUT = 3,
	HEREDOC = 4,
	APPEND = 5,
	WORD = 6,
	SPACE = 7,
	END = 8,
	EMPTY = 9
}	t_type;

typedef enum e_err
{
	ARGS_NUM,
	INV_ARGS,
	MLLC,
	NO_FD = -1,
	DEF = -2,
}	t_err;

typedef enum e_print
{
	AMBIG,
	PERM_DEN,
	NO_FILE,
	DIRECT,
	HERED_ERR,
	HIST_RD_ERR,
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
	MALLOC_ERR = 200,
	SYSTEM_ERR = 201,
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

# define OWN_ERR_MSG "ms error: "
# define HISTORY_FILE ".history.tmp"
# define ERR_MALLOC "memory allocation failed\n"
# define NO_FD_ERR ": No such file or directory\n"
# define IS_DIR_ERR ": Is a directory\n"
# define PERM_DEN_ERR ": Permission denied\n"
# define NO_CMD_ERR ": command not found\n"
# define AMBIG_ERR ": ambiguos redirect\n"
# define HISTORY_RD_ERR "Reading from history file failed\n"
# define FLAG_ERR ": flags are not supported in minishell\n"
# define INDENT_ERR "': not a valid identifier\n"
# define BLOCKS_ERR "Error: failed to create blocks\n"
# define CMDS_ERR "Error: failed to create commands\n"
# define TOKENS_ERR "Error: failed to create tokens\n"
# define HEREDOC_ERR "maximum here-document count exceeded\n"
# define HISTORY_SIZE 500

#endif
