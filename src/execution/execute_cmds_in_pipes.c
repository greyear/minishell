#include "../../include/minishell.h"

void    create_pipes(int num_cmds, int ***pipe_fd)
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
}

void    close_pipes(int num_cmds, int **pipe_fd)
{
    int     i;

    i = 0;
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
    if (pipe_fd)
        free(pipe_fd);
}

void    wait_for_children(int num_cmds, pid_t last_pid, t_ms *ms)
{
    int     i;
    int     status;
    pid_t   wpid;

    i = 0;
    while (i < num_cmds)
    {
        wpid = wait(&status);
        if (wpid == last_pid && WIFEXITED(status))
            ms->exit_status = WEXITSTATUS(status);
        i++;
    }
}

void    pipe_or_redir(t_cmd *cur, int **pipe_fd, int i, int num_cmds)
{
    if (cur->infile == NO_FD || cur->outfile == NO_FD)
        exit(1);
    if (i == 0)
        pipe_process(NULL, pipe_fd[i]);
    else if (i == num_cmds - 1)
        pipe_process(pipe_fd[i-1], NULL);
    else
        pipe_process(pipe_fd[i-1], pipe_fd[i]);
    redirect_process(cur->infile, cur->outfile);
}

/*void    make_multiple_childs(int num_cmds, t_cmd *cmds, t_ms *ms)
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
            pipe_or_redir(cur, pipe_fd, i, num_cmds);
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
    cleanup_heredocs(ms->heredoc_files);
    reset_heredocs(ms);
    wait_for_children(num_cmds, last_pid, ms);
}*/

void setup_pipes(t_exec_data *data)
{
    create_pipes(data->num_cmds, &data->pipe_fd);
    if (!data->pipe_fd)
        data->ms->exit_status = 1;
}

void	fork_and_execute(t_cmd *cur, int i, t_exec_data *data)
{
    pid_t pid;

    pid = fork();
    if (pid < 0)
    {
        data->ms->exit_status = 1;
        return;
    }
    if (pid == 0)
    {
        pipe_or_redir(cur, data->pipe_fd, i, data->num_cmds);
        if (is_builtin(cur))
        {
            handle_builtin(cur, data->ms, 1);
            exit(data->ms->exit_status);
        }
        else
            execute_command(data->ms->envp, cur->args);
    }
    if (i > 0 && data->pipe_fd[i - 1])
    {
        close(data->pipe_fd[i - 1][0]);
        close(data->pipe_fd[i - 1][1]);
    }
    data->last_pid = pid;
}

void	cleanup_after_execution(t_exec_data *data)
{
	close_pipes(data->num_cmds, data->pipe_fd);
    wait_for_children(data->num_cmds, data->last_pid, data->ms);
    cleanup_heredocs(data->ms->heredoc_files);
    data->ms->heredoc_files = malloc(sizeof(char *) * 100);
    ft_memset(data->ms->heredoc_files, 0, sizeof(char *) * 100);
    data->ms->heredoc_count = 0;
}

void    make_multiple_childs(int num_cmds, t_cmd *cmds, t_ms *ms)
{
    t_exec_data data;
    t_cmd   *cur;
    int     i;

    i = 0;
    cur = cmds;
    data.num_cmds = num_cmds;
    data.ms = ms;
    data.pipe_fd = NULL;
    data.last_pid = -1;
    setup_pipes(&data);
    if (!data.pipe_fd)
        return;
    while (i < num_cmds && cur)
    {
        if (!cur->args || !cur->args[0])
        {
            data.ms->exit_status = 0;
            cur = cur->next;
            i++;
            continue;
        }
        fork_and_execute(cur, i, &data);
        cur = cur->next;
        i++;
    }
    cleanup_after_execution(&data);
}
