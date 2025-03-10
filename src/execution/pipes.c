#include "../../include/minishell.h"

/**
 * @brief Sets up the pipes for input and output redirection in a process.
 * 
 * This function handles the redirection of the standard input (STDIN) and standard output (STDOUT) for a process
 * based on the provided pipe file descriptors. If a previous pipe is provided, its read-end is duplicated to STDIN.
 * If a next pipe is provided, its write-end is duplicated to STDOUT. The corresponding pipe file descriptors are
 * closed after duplication to ensure proper cleanup.
 * 
 * @param prev_pipe A pointer to an array of integers representing the file descriptors of the previous pipe,
 *                  where `prev_pipe[0]` is the read-end and `prev_pipe[1]` is the write-end. If `NULL`, no input redirection is applied.
 * @param next_pipe A pointer to an array of integers representing the file descriptors of the next pipe,
 *                  where `next_pipe[0]` is the read-end and `next_pipe[1]` is the write-end. If `NULL`, no output redirection is applied.
 * 
 * @return This function does not return; it modifies the process’s standard input and output as needed.
 */

void	pipe_process(int prev_pipe, int next_pipe)
{
	if (prev_pipe > 0)
	{
		if (dup2(prev_pipe, STDIN_FILENO) == -1) //It duplicates previous pipes read-end to stadard input
		{
			close(prev_pipe);
			exit(1);
		}	
		//close(prev_pipe);
	}
	if (next_pipe > 0)
	{
		if (dup2(next_pipe, STDOUT_FILENO) == -1) //It duplicates the next pipes write-end to standard output
		{
			close(next_pipe);
			exit(1);
		}	
		//close(next_pipe);
	}
}

/**
 * @brief Handles pipe and redirection setup for a command in a pipeline.
 * 
 * This function manages input/output redirection and sets up pipes for processes in a pipeline. If the command
 * requires input or output redirection, it is handled using `redirect_process`. If the command is not the first or
 * last in the pipeline, the function connects the previous and next processes using pipes by calling `pipe_process`.
 * The appropriate pipes are set up depending on whether the command is the first, last, or middle in the pipeline.
 * 
 * @param cur A pointer to the `t_cmd` structure representing the current command, which includes the input and output file descriptors.
 * @param pipe_fd A pointer to a 2D array of integers representing the pipes between commands in the pipeline.
 * @param i The index of the current command in the pipeline, used to determine its position for pipe setup.
 * @param num_cmds The total number of commands in the pipeline, used to determine if the current command is the first or last.
 * 
 * @return This function does not return; it modifies the process’s input/output redirection and sets up pipes if necessary.
 */

void    pipe_or_redir(t_cmd *cur, int *pipe_fd, int i, int num_cmds, int cur_fd)
{
    if (cur->infile == NO_FD || cur->outfile == NO_FD)
        exit(1);
    if (i == 0)
        pipe_process(0, pipe_fd[1]);
    else if (i == num_cmds - 1)
        pipe_process(cur_fd, 0);
    else
        pipe_process(cur_fd, pipe_fd[1]);
    //if (cur_fd != -1)
    //    close(cur_fd);
    //close(pipe_fd[1]);
    redirect_process(cur->infile, cur->outfile);
}

/**
 * @brief Allocates and creates pipes for inter-process communication in a pipeline.
 * 
 * This function dynamically allocates memory for a 2D array of pipe file descriptors, where each row represents
 * a pipe between two consecutive commands in a pipeline. It creates `num_cmds - 1` pipes, as each pipe connects two
 * commands. If any memory allocation or pipe creation fails, it frees the allocated memory and sets the pipe array to `NULL`.
 * 
 * @param num_cmds The total number of commands in the pipeline, used to determine how many pipes are needed.
 * @param pipe_fd A pointer to a pointer to a 2D array of integers, which will hold the pipe file descriptors for communication.
 * 
 * @return This function does not return; it modifies the `pipe_fd` array by allocating and setting up the pipes.
 */

/*static void    create_pipes(int num_cmds, int ***pipe_fd)
{
    int     i;

    i = 0;
    *pipe_fd = malloc(sizeof(int *) * (num_cmds - 1));
    if (!*pipe_fd)
        return;
    while (i < num_cmds - 1)
    {
        (*pipe_fd)[i] = malloc(sizeof(int) * 2);
        if (!(*pipe_fd)[i])
        {
            free_int_array(*pipe_fd);
            *pipe_fd = NULL;
            return;
        }
        if (pipe((*pipe_fd)[i]) == -1)
        {
            free_int_array(*pipe_fd);
            *pipe_fd = NULL;
            return;
        }
        i++;
    }
}*/

/**
 * @brief Sets up pipes for inter-process communication in a pipeline.
 * 
 * This function calls `create_pipes` to allocate and create the necessary pipes for the given number of commands in
 * the pipeline. If pipe creation fails (i.e., the pipe file descriptors are not allocated), it sets the global exit
 * status to 1 to indicate an error.
 * 
 * @param data A pointer to the `t_exec_data` structure, which contains the number of commands (`num_cmds`) and
 *             a pointer to the pipe file descriptors (`pipe_fd`). The function modifies the `pipe_fd` field and the
 *             `exit_status` of the `ms` field in the `data` structure if an error occurs.
 * 
 * @return This function does not return; it modifies the `pipe_fd` array and the global exit status as necessary.
 */

/*void	setup_pipes(t_pipe *p)
{
    create_pipes(p->num_cmds, &p->fd);
    if (!p->fd)
        p->ms->exit_status = 1;
}*/