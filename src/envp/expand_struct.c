#include "../../include/minishell.h"

/**
 * @brief Allocates and initializes a t_expand structure for variable 
 * expansion.
 * 
 * This function dynamically allocates memory for a `t_expand` structure, which 
 * is used to store the parameters needed for variable expansion. It 
 * initializes all fields to default values to ensure safe usage.
 * 
 * If memory allocation fails, an error message is printed using `perror`, and 
 * the program terminates with `exit(1)`.
 * 
 * @return A pointer to the newly allocated and initialized `t_expand` 
 *         structure.
 */
t_expand	*exp_init(t_ms *ms)
{
	t_expand	*exp;

	exp = malloc(sizeof(t_expand));
	if (!exp)
		return (print_malloc_set_status(ms));
	exp->data = NULL;
	exp->key = NULL;
	exp->len = 0;
	exp->quote = 0;
	exp->if_first = 0;
	exp->expanded = false;
	return (exp);
}

/**
 * @brief Initializes a `t_expand` structure for variable expansion.
 * 
 * This function allocates and initializes a `t_expand` structure, which is used 
 * for handling variable expansion in tokens. If memory allocation fails, 
 * an error message is printed using `print_malloc_set_status(ms)`.
 * 
 * @param ms A pointer to the main shell structure for error handling.
 * 
 * @return A pointer to the newly allocated `t_expand` structure, or `NULL` if 
 *         memory allocation fails.
 */
t_expand	*initialize_expansion(t_ms *ms)
{
	t_expand	*exp;

	exp = exp_init(ms);
	if (!exp)
		print_malloc_set_status(ms);
	return (exp);
}
