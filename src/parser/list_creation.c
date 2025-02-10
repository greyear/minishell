
#include "../../include/minishell.h"

/*
struct t_command (cmd_name, cmd_ar, red_in, red_out, index, next)

count_words: counts the number of word tokens in list between A and B
fill_command_array: checks all tokens between A and B and if the type of the token is word 
					then puts the copy of content of it into the array
set_values:: sets default values to t_command *new
form_command: counts number of words (between pipes?), allocates memory for the command struct,
			  sets default values, allocates memory for the arguments (field of cmd), fills
			  the array of arguments, separately puts cmd name in specific field
form_command_list: 
*/