#include "../../include/minishell.h"

/**
 * @brief Waits for all child processes to finish and updates the exit status of the last child process.
 * 
 * This function waits for a number of child processes to finish based on the `num_cmds` argument. It checks the status
 * of each child process, and if the last child process (identified by `last_pid`) has exited successfully, it updates
 * the `exit_status` in the `ms` structure with the exit status of that process.
 * 
 * @param num_cmds The total number of child processes to wait for.
 * @param last_pid The process ID of the last child process. The exit status of this process is recorded.
 * @param ms A pointer to the `t_ms` structure where the exit status of the last child process is stored.
 * 
 * @return This function does not return; it waits for the specified child processes and updates the `exit_status`.
 */

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

static void child_process(t_cmd *cur, t_pipe *p)
{
    setup_pipes(p->fd, p->cmd_num, p->num_cmds, p->cur_fd);
    redirect_process(cur->infile, cur->outfile);
    close_all_fds(p, p->ms);
    if (is_builtin(cur))
    {
        handle_builtin(cur, p->ms, 1);
        exit(p->ms->exit_status);
    }
    else
        execute_command(p->ms->envp, cur->args);
}

static void fork_and_execute(t_cmd *cur, t_pipe *p)
{
    if (pipe(p->fd) == -1)
    {
        perror("pipe failed");
        exit(1);
    }
    p->pids[p->cmd_num] = fork(); 
    if (p->pids[p->cmd_num] < 0)
    {
        p->ms->exit_status = 1;
        return;
    }
    if (p->pids[p->cmd_num] == 0)
        child_process(cur, p);
    close_fds2(p->cur_fd, p->fd[1]);
    p->cur_fd = p->fd[0];
    p->last_pid = p->pids[p->cmd_num];
}

void    initialize_p(t_pipe *p, int num_cmds, t_ms *ms)
{
    p->num_cmds = num_cmds;
    p->ms = ms;
    p->last_pid = -1;
    p->cmd_num = 0;
    p->cur_fd = -1;
    p->pids = (pid_t *)malloc((p->num_cmds) * sizeof(pid_t));
}

void    make_multiple_childs(int num_cmds, t_cmd *cmds, t_ms *ms)
{
    t_pipe  p;
    t_cmd   *cur;

    cur = cmds;
    initialize_p(&p, num_cmds, ms);
    if (!p.pids)
        return;
    while (p.cmd_num < p.num_cmds && cur)
    {
        if (!cur->args || !cur->args[0])
        {
            p.ms->exit_status = 0;
            cur = cur->next;
            p.cmd_num++;
            continue;
        }
        fork_and_execute(cur, &p);
        cur = cur->next;
        p.cmd_num++;
    }
    close_all_fds(&p, ms);
    wait_for_children(p.num_cmds, p.last_pid, p.ms);
    cleanup_after_execution(&p);
}
