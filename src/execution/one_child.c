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

#include "../../include/minishell.h"

void	pipe_process(int *prev_pipe, int *next_pipe)
{
	if (prev_pipe)
	{
		if (dup2(prev_pipe[0], STDIN_FILENO) == -1) //It duplicates previous pipes read-end to stadard input
		{
			close(prev_pipe[0]);
			close(prev_pipe[1]);
			exit(1);
		}	
		close(prev_pipe[0]);
		close(prev_pipe[1]);
	}
	if (next_pipe)
	{
		if (dup2(next_pipe[1], STDOUT_FILENO) == -1) //It duplicates the next pipes write-end to standard output
		{
			close(next_pipe[0]);
			close(next_pipe[1]);
			exit(1);
		}	
		close(next_pipe[0]);
		close(next_pipe[1]);
	}
}

void	redirect_process(int infile, int outfile)
{
	if (infile != NO_FD && infile != DEF)
	{
		if (dup2(infile, STDIN_FILENO) == -1) //It duplicates previous pipes read-end to stadard input
		{
			close(infile);
			exit(1);
		}
		close(infile);
	}
	if (outfile != NO_FD && outfile != DEF)
	{
		if (dup2(outfile, STDOUT_FILENO) == -1) //It duplicates the next pipes write-end to standard output
		{
			close (outfile);
			exit(1);
		}
		close(outfile);
	}
}

void	execute_child(t_cmd *cmd, t_ms *ms)
{
	redirect_process(cmd->infile, cmd->outfile);
    if (is_builtin(cmd))
    {
        handle_builtin(cmd, ms, 1);
        exit(ms->exit_status);
    }
    else
        execute_command(ms->envp, cmd->args);
}

void	reset_heredocs(t_ms *ms)
{
	ms->heredoc_files = malloc(sizeof(char *) * 100);
    ft_memset(ms->heredoc_files, 0, sizeof(char *) * 100);
    ms->heredoc_count = 0;
}

void	make_one_child(t_cmd *cmd, t_ms *ms)
{
    pid_t	pid;
    int		status;

    ms->exit_status = 0;
    if (!cmd->args || !cmd->args[0])
        return;
    if (cmd->infile == NO_FD || cmd->outfile == NO_FD)
    {
        ms->exit_status = 1;
        return;
    }
	pid = fork();
    if (pid < 0)
    {
        ms->exit_status = 1;
        return;
    }
    if (pid == 0)
        execute_child(cmd, ms);
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
		ms->exit_status = WEXITSTATUS(status);
	cleanup_heredocs(ms->heredoc_files);
	reset_heredocs(ms);
}
