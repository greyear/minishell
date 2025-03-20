#include "../../include/minishell.h"

/**
 * @brief Frees a dynamically allocated array of strings and sets it to NULL.
 * 
 * This function iterates through a null-terminated array of strings, freeing 
 * each individual string and then freeing the array itself. It ensures that 
 * the array pointer is set to NULL to prevent dangling pointers.
 * 
 * @param arr A pointer to the array of strings to be freed. The function 
 *            modifies this pointer, setting it to NULL after deallocation.
 */
void	clean_arr(char ***arr)
{
	int		i;

	if (!arr || !*arr)
		return ;
	i = 0;
	while ((*arr)[i])
	{
		free((*arr)[i]);
		(*arr)[i] = NULL;
		i++;
	}
	free(*arr);
	*arr = NULL;
}
