#include "../../include/minishell.h"

/**
 * @brief Counts the number of heredoc tokens in the token list.
 * 
 * This function iterates through the token list and counts how many tokens 
 * are of type `HEREDOC`. The function returns the total count of heredoc 
 * tokens in the list.
 * 
 * @param token A pointer to the head of the token list to be scanned.
 * 
 * @return The number of heredoc tokens found in the list.
 */
static int	count_heredocs(t_token *token)
{
	t_token	*cur;
	int		heredoc_count;

	cur = token;
	heredoc_count = 0;
	while (cur)
	{
		if (cur->type == HEREDOC)
			heredoc_count++;
		cur = cur->next;
	}
	return (heredoc_count);
}

/**
 * @brief Allocates memory for heredoc file paths and handles errors.
 * 
 * This function calculates the number of heredocs from the given token list 
 * and allocates memory for storing the paths to the heredoc files in the 
 * `ms->heredoc_files` array. If the number of heredocs exceeds the maximum 
 * allowed (16), an error message is printed, and the program exits. If memory 
 * allocation fails, a malloc error is printed, and the program exits. 
 * Otherwise, the function initializes the `ms->heredoc_files` array with NULL 
 * values.
 * 
 * @param ms A pointer to the main shell structure, which will store the heredoc
 *           file paths.
 * @param token A pointer to the token list, which is used to count the heredocs.
 */
static void	malloc_heredocs(t_ms *ms, t_token *token)
{
	int		heredoc_count;

	heredoc_count = count_heredocs(token);
	if (heredoc_count > 16)
	{
		ft_putstr_fd(OWN_ERR_MSG, STDERR_FILENO);
		ft_putstr_fd(HEREDOC_ERR, STDERR_FILENO);
		clean_struct(ms);
		exit(2);
	}
	ms->heredoc_files = malloc(sizeof(char *) * (heredoc_count + 1));
	if (!ms->heredoc_files)
	{
		print_malloc_error();
		clean_struct(ms);
		exit(1);
	}
	ft_memset(ms->heredoc_files, 0, sizeof(char *) * (heredoc_count + 1));
	ms->heredoc_files[0] = NULL;
}

/**
 * @brief Tokenizes the input string and processes redirections and heredocs.
 * 
 * This function tokenizes the given input string and stores the tokens in the 
 * `ms->tokens` structure. If tokenization fails, it prints an error message and 
 * returns 0. It also frees the original input string after tokenization. After 
 * tokenization, it processes heredocs and sets up files for redirections.
 * 
 * @param input A pointer to the input string to be tokenized.
 * @param ms A pointer to the main shell structure, which holds the tokens.
 * @return 1 if tokenization is successful, 0 if there is an error.
 */
int	tokenize_input(char **input, t_ms *ms)
{
	ms->tokens = tokenization(*input, ms);
	free(*input);
	if (!ms->tokens)
	{
		ft_putstr_fd(TOKENS_ERR, STDERR_FILENO);
		return (0);
	}
	malloc_heredocs(ms, ms->tokens);
	put_files_for_redirections(ms->tokens);
	return (1);
}

/**
 * @brief Creates the blocks and commands lists from the tokenized input.
 * 
 * This function creates the list of blocks and commands from the tokenized 
 * input. If an error occurs during the creation of either list, it will 
 * clean up any previously allocated resources (tokens and blocks) and return 
 * 0. Otherwise, it returns 1 to indicate successful creation of both lists.
 * 
 * @param ms A pointer to the main shell structure, used to store the blocks 
 *           and commands lists.
 * @return 1 if both blocks and commands lists are successfully created, 
 *         0 if there was an error.
 */
int	create_blocks_and_cmds_lists(t_ms *ms)
{
	int		err_syntax;

	err_syntax = 0;
	ms->blocks = create_blocks_list(ms->tokens, NULL, &err_syntax);
	if (err_syntax)
	{
		ft_putstr_fd(BLOCKS_ERR, STDERR_FILENO);
		clean_token_list(&(ms->tokens));
		return (0);
	}
	ms->cmds = create_cmd_list(ms->blocks, ms);
	if (!ms->cmds)
	{
		ft_putstr_fd(CMDS_ERR, STDERR_FILENO);
		clean_token_list(&(ms->tokens));
		clean_block_list(&(ms->blocks));
		return (0);
	}
	return (1);
}
