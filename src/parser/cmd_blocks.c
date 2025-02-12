
#include "../../include/minishell.h"

int	check_block(t_token *start, t_token *end, int *err_flag) //check without
{
	t_token	*cur;
	//what if nothing in between
	cur = start;
	while (cur != end)
	{
		if (!(cur->type == WORD || is_redirect(cur->type)))
		{
			*err_flag = 1; //save place?
			return (1);
		}
		cur = cur->next;
	}
	return (0);
}

//creates new cmd_block and puts to the end of list of blocks starting from block
t_block	*create_block(t_token *start, t_token *end, t_block *first_block, int *err_flag)
{
	t_block	*new;
	t_block	*cur;

	if (check_block(start, end, err_flag))
		return (first_block);
	new = (t_block *)malloc(1 * sizeof(t_block));
	if (!new)
	{
		*err_flag = 1; //?
		return (first_block);
	}
	new->start = start;
	new->end = end;
	new->next = NULL;
	if (!first_block) //if blocks list is still empty
		return (new);
	cur = first_block;
	while (cur->next)
		cur = cur->next;
	cur->next = new; //put it as a last
	return (first_block);
}

//check r!

t_block	*create_blocks_list(t_token *start, t_token *end, int *err_flag)
{
	t_token	*cur_token;
	t_block	*first_block;

	first_block = NULL;
	cur_token = start;
	while ()
	{
		if (cur_token->type == PIPE)
		{
			first_block = create_block(start, cur_token, first_block, err_flag);
			start = cur_token->next; //switch to the next one
		}
		cur_token = cur_token->next;
		//err_flag check
	}

	return (first_block);
}

/*
struct t_command (cmd_name, cmd_ar, red_in, red_out, index, next)
struct t_pipeline (t_token *start, t_token *end, next)
struct t_heredoc (index, filename, next)
struct t_ast (t_pipeline *, t_command *, token_type, t_ast *up/right/left, ...)

count_words: counts the number of word tokens in list between A and B
fill_command_array: checks all tokens between A and B and if the type of the token is word 
					then puts the copy of content of it into the array
set_values:: sets default values to t_command *new
form_command: counts number of words (between pipes?), allocates memory for the command struct,
			  sets default values, allocates memory for the arguments (field of cmd), fills
			  the array of arguments, separately puts cmd name in specific field
			  handles redirections
form_command_list: creates a linked list of commands (calls form_command), if function call
				   returns an error, cleans the whole list
incorrect_token_in_pipeline: checks all tokens between A and B and if they are 
							 NOT words and NOT redirections, returns an error
form_pipeline_node: calls token check between A and B and then
					creates t_pipeline node which points to A and B tokens between pipes
					and adds it at the end of t_pipeline list!
check_last_segment: //////////////////////////////////
form_pipeline: goed through token_list, finds pipes and forms pipeline list

make_command_node: creates AST node
remove_braces: auxilary for branch_out
find_logic_token: finds &&, || or parenthes
branch_out: decides which node should be added
form_tree: builds AST tree


*/