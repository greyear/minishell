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
	int	i;

	if (!arr || !*arr)
		return;
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

void	free_int_array(int **array) //do we need it??
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	cleanup_heredocs(char **filenames)
{
	int	i;
	
	i = 0;
	if (!filenames)
		return;
	while (filenames[i])
	{
		if (unlink(filenames[i]) == -1)
		{
			perror("unlink fail"); //ms->exit_status = SYSTEM_ERR;? 
			exit(1); //?
		}
		free(filenames[i]);
		i++;
	}
	if (filenames)
		free(filenames);
}

/**
 * @brief Frees the allocated memory for process IDs in a pipeline structure.
 * 
 * This function checks if the `p->pids` array exists and frees it to 
 * prevent memory leaks. It then sets the pointer to `NULL` to avoid 
 * accidental access to freed memory.
 * 
 * @param p A pointer to the `t_pipe` structure containing the process ID array.
 */
void	free_pids(t_pipe *p)
{
	if (p->pids)
	{
		free(p->pids);
		p->pids = NULL;
	}
}