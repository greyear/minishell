/* 
Idea is to replicate the functionality of Unix pipes,
where commands are chained together with |, passing the output of one command as input to the next
execute_cmd function takes command amount, commands as **array and envp as arguments
EXAMPLE:if you need it to work like ls -l | grep "Error" | sort
send these as arguments to ft_pipe
int num_cmds = 3;
char **cmds = cmds[0]="ls -l", cmds[1] ="grep Error", cmds[2] ="sort", cmds[3]=NULL
char **envp = environmental variables

FUNCTIONALITY:
Creates pipes and forks child processes for each command.
If only single command, executes it 
Fork splits the process to child and parent process, child processes execute the commands,
The parent process waits for all the child processes to complete.
Pipe makes the pipe.
Pipe allows fle descriptors to communicate.
A process writes data to the pipe_fd's write-end (pipe_fd[1]),
which can then be read by another process from the pipe_fd's read-end (pipe_fd[0].
In a typical shell pipeline (e.g., cmd1 | cmd2):
The output of cmd1 is sent to cmd2 through a pipe.
cmd1 writes its output to the write end of the pipe.
cmd2 reads from the read end of the pipe.

ALSO WORKS WITH SINGLE COMMAND EXECUTION SO CALL THIS WHENEVER YOU NEED TO EXECUTE!!!
*/

//include "seela.h"
#include "../../include/minishell.h"

static void	pipe_process(int *prev_pipe, int *next_pipe)
{
	if (prev_pipe)
	{
		if (dup2(prev_pipe[0], STDIN_FILENO) == -1) //It duplicates previous pipes read-end to stadard input
			exit(1);
		close(prev_pipe[0]);
		close(prev_pipe[1]);
	}
	if (next_pipe)
	{
		if (dup2(next_pipe[1], STDOUT_FILENO) == -1) //It duplicates the next pipes write-end to standard output
			exit(1);
		close(next_pipe[0]);
		close(next_pipe[1]);
	}
}

void	execute_single_cmd(t_cmd *cmd, t_ms *ms)
{
	pid_t	pid;
	int		status;

	ms->exit_status = 0;
	pid = fork();
	if (pid < 0)
	{
		ms->exit_status = 1;
		return;
	}
	if (pid == 0) // Child process
	{
		if (is_builtin(cmd))
		{
			handle_builtin(cmd, ms, 0);
			exit(ms->exit_status);
		}
		else
		{
			ft_command(ms->envp, cmd->args); // Execute command
			exit(127); // Only reached if execve fails
		}
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		ms->exit_status = WEXITSTATUS(status);
}

void	free_int_array(int **array)
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


void	execute_cmd(int num_cmds, t_cmd *cmds, t_ms *ms)
{
	//int		pipe_fd[num_cmds - 1][2];
	int		**pipe_fd;
	int		i;
	pid_t	pid;
    pid_t   last_pid;
    pid_t   wpid;
    int     status;
	t_cmd	*cur;

    i = 0;
    last_pid = -1;
	if (num_cmds == 1) // Handle single command case
    {
        execute_single_cmd(cmds, ms);
        return;
    }
	pipe_fd = malloc(sizeof(int *) * (num_cmds - 1));
	if (!pipe_fd)
		return;
	cur = cmds;
	while (cur)//(i < num_cmds)
	{
		pipe_fd[i] = malloc(sizeof(int) * 2);
		if (!pipe_fd[i])
		{
			free_int_array(pipe_fd);
			return;
		}
		if (i < num_cmds - 1)
		{
			if (pipe(pipe_fd[i]) == -1)
			{
				ms->exit_status = 1;
		        return;
			}
		}
		pid = fork();
		if (pid < 0)
		{
			ms->exit_status = 1;
			return;
		}
		if (pid == 0) // Child process
		{
			if (i == 0) // first command
				pipe_process(NULL, pipe_fd[i]);
			else if (i == num_cmds - 1) // last command
				pipe_process(pipe_fd[i - 1], NULL);
			else //any commands in between
				pipe_process(pipe_fd[i - 1], pipe_fd[i]);
			if (is_builtin(cur))
			{
				handle_builtin(cur, ms, 1);
				exit(ms->exit_status);
			}
			else
				ft_command(ms->envp, cur->args); //execute command
		}
		if (i > 0)
		{
			close(pipe_fd[i - 1][0]);
			close(pipe_fd[i - 1][1]);
		}
        last_pid = pid;
		cur = cur->next;
		i++;
	}
	i = 0;
	while (i < num_cmds - 1)
	{
		close(pipe_fd[i][0]);
 		close(pipe_fd[i][1]);
		free(pipe_fd[i]);
		i++;
	}
	free(pipe_fd);
	i = 0;
	while (i < num_cmds)
	{
        wpid = wait(&status);
		if (wpid == last_pid && WIFEXITED(status))
            ms->exit_status = WEXITSTATUS(status); //use this in echo ?$
		i++;
	}
}
