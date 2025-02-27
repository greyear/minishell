
#include "../../include/minishell.h"

/**
 * @brief Determines the type of a token based on a given character in a string.
 * 
 * This function checks a character (and sometimes the next character) to 
 * classify it as a specific token type. It distinguishes between different 
 * types of redirections, pipes, spaces, and words.
 * 
 * @param str The input string containing the character to be analyzed.
 * @param i The index of the character in the string to classify.
 * 
 * @return The corresponding token type (`HEREDOC`, `IN`, `APPEND`, `OUT`, 
 *         `PIPE`, `SPACE`, or `WORD`).
 */
t_token_type	define_token_type(char *str, size_t i)
{
	//&?
	if (str[i] == '<' && str[i + 1] == '<')
		return (HEREDOC);
	else if (str[i] == '<')
		return (IN);
	else if (str[i] == '>' && str[i + 1] == '>')
		return (APPEND);
	else if (str[i] == '>')
		return (OUT);
	else if (str[i] == '|' && str[i + 1] != '|') // do we need to check the second | or if we already checked it in BNF it's unnecessary?
		return (PIPE);
	else if (ft_isspace(str[i]))
		return (SPACE);
	else
		return (WORD);
}

/**
 * @brief Initializes a t_token structure with default values.
 * 
 * This function sets all fields of the given token to their default states, 
 * ensuring proper initialization before further use. It prevents 
 * uninitialized memory issues and ensures predictable behavior.
 * 
 * @param new A pointer to the t_token structure to be initialized.
 */
static void	default_token_values(t_token *new)
{
	new->data = NULL;
	new->file = NULL;
	new->next = NULL;
	new->quote = 0;
	new->unclosed = false; //check in debugger if we need it
	new->specific_redir = EMPTY;
	new->ambiguous = false;
	//some other fields
}

/**
 * @brief Creates a new token and initializes its values.
 * 
 * This function allocates memory for a new t_token structure, assigns its type, 
 * and extracts the corresponding data from the input string. If the token is 
 * a WORD, it handles quoted and unquoted words separately. If it is a SPACE 
 * or a special token (like a redirection or pipe), it skips the appropriate 
 * characters.
 * 
 * @param str The input string from which the token is extracted.
 * @param i A pointer to the current position in the string. This value is 
 *          updated as the function progresses through the input.
 * @param type The type of the token being created.
 * 
 * @return A pointer to the newly created t_token structure, or NULL in case 
 *         of a memory allocation failure.
 */
t_token	*create_new_token(char *str, size_t *i, t_token_type type)
{
	t_token	*new;

	new = (t_token *)ft_calloc(1, sizeof(t_token));
	if (!new)
		return (NULL); //error?
	default_token_values(new);
	new->type = type;
	if (type == WORD)
	{
		if (str[*i] == SG_QUOT || str[*i] == DB_QUOT)
		{
			new->quote = str[*i];
			new->data = word_with_quotes(str, i, new);
		}
		else
		{
			new->data = word_without_quotes(str, i);
			//printf("created new->data\n");
		}
			
		if (!new->data)
			return (NULL); //error?
	}
	else if (type == SPACE)
		skip_whitespaces(str, i);
	else
		skip_special_tokens(str, i, type);
	return (new);
}

//define type -> check size -> for all except spaces and words ++ -> for word ++ and put content
//


