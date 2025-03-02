#include "../../include/minishell.h"

void    create_pipes(int num_cmds, int ***pipe_fd)
{
    int i = 0;
    
    *pipe_fd = malloc(sizeof(int *) * (num_cmds - 1));
    if (!*pipe_fd)
        return;
    while (i < num_cmds - 1)
    {
        (*pipe_fd)[i] = malloc(sizeof(int) * 2);
        if (!(*pipe_fd)[i] || pipe((*pipe_fd)[i]) == -1)
        {
            free_int_array(*pipe_fd);
            *pipe_fd = NULL;
            return;
        }
        i++;
    }
}

void    close_pipes(int num_cmds, int **pipe_fd)
{
    int i = 0;

    while (i < num_cmds - 1)
    {
        if (pipe_fd[i])
        {
            close(pipe_fd[i][0]);
            close(pipe_fd[i][1]);
            free(pipe_fd[i]);
        }
        i++;
    }
    free(pipe_fd);
}

void    wait_for_children(int num_cmds, pid_t last_pid, t_ms *ms)
{
    int i = 0;
    int status;
    pid_t wpid;

    while (i < num_cmds)
    {
        wpid = wait(&status);
        if (wpid == last_pid && WIFEXITED(status))
            ms->exit_status = WEXITSTATUS(status);
        i++;
    }
}

void    make_multiple_childs(int num_cmds, t_cmd *cmds, t_ms *ms)
{
    int     **pipe_fd;
    pid_t   last_pid;
    pid_t   pid;
    t_cmd   *cur;
    int     i;

    i = 0;
    last_pid = -1;
    create_pipes(num_cmds, &pipe_fd);
    if (!pipe_fd)
    {
        ms->exit_status = 1;
        return;
    }
    cur = cmds;
    while (i < num_cmds && cur)
    {
        if (!cur->args || !cur->args[0])
        {
            ms->exit_status = 0;
            cur = cur->next;
            i++;
            continue;
        }
        pid = fork();
        if (pid < 0)
        {
            ms->exit_status = 1;
            return;
        }
        if (pid == 0)
        {
            if (cur->infile == NO_FD || cur->outfile == NO_FD)
                exit(1); //not sure about that!
            if (i == 0) // First command
                pipe_and_redir(cur->infile, cur->outfile, NULL, pipe_fd[i]);
            else if (i == num_cmds - 1) // Last command
                pipe_and_redir(cur->infile, cur->outfile, pipe_fd[i-1], NULL);
            else // Any commands in between
                pipe_and_redir(cur->infile, cur->outfile, pipe_fd[i-1], pipe_fd[i]);
            if (is_builtin(cur))
            {
                handle_builtin(cur, ms, 1);
                exit(ms->exit_status);
            }
            else
                execute_command(ms->envp, cur->args); //execute command
        }
        if (i > 0 && pipe_fd[i - 1])
        {
            close(pipe_fd[i - 1][0]);
            close(pipe_fd[i - 1][1]);
        }
        last_pid = pid;
        cur = cur->next;
        i++;
    }
    close_pipes(num_cmds, pipe_fd);
    wait_for_children(num_cmds, last_pid, ms);
    cleanup_heredocs(ms->heredoc_files);
    ms->heredoc_files = malloc(sizeof(char *) * 100);
    ft_memset(ms->heredoc_files, 0, sizeof(char *) * 100);
    ms->heredoc_count = 0;
}