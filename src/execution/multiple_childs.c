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

/**
 * @brief Creates a child process to execute a command, handling pipes, redirections, and built-ins.
 * 
 * This function forks the process to create a child. In the child process, it first sets up the necessary pipes
 * or redirections using `pipe_or_redir`, then checks if the command is a built-in. If it is a built-in, the
 * appropriate handler is called, and the child process exits with the corresponding exit status. If the command
 * is not a built-in, the command is executed using `execute_command`. In the parent process, it closes the pipe
 * file descriptors for previous commands (if any) and updates the `last_pid` to the child process ID.
 * 
 * @param cur A pointer to the `t_cmd` structure representing the current command to be executed.
 * @param i The index of the current command, used to manage pipes and redirections.
 * @param p A pointer to the `t_exec_data` structure that holds the execution p, including pipes,
 *             environment variables, and the last process ID.
 * 
 * @return This function does not return; it either forks a new process to execute the command or handles errors.
 */

static void fork_and_execute(t_cmd *cur, t_pipe *p)
{
    //pid_t pid;

    //pid = fork();
    if (pipe(p->fd) == -1)
        exit(20);
    p->pids[p->cmd_num] = fork(); 
    if (p->pids[p->cmd_num] < 0)
    {
        p->ms->exit_status = 1;
        return;
    }
    if (p->pids[p->cmd_num] == 0)
    {
        pipe_or_redir(cur, p->fd, p->cmd_num, p->num_cmds);
        if (is_builtin(cur))
        {
            handle_builtin(cur, p->ms, 1);
            exit(p->ms->exit_status);
        }
        else
            execute_command(p->ms->envp, cur->args);
    }
    p->cur_fd = p->fd[0];
    /*if (i > 0 && p->pipe_fd[i - 1])
    {
        close(p->pipe_fd[i - 1][0]);
        close(p->pipe_fd[i - 1][1]);
    }*/
    p->last_pid = p->pids[p->cmd_num];
}

/**
 * @brief Creates multiple child processes to execute a series of commands, handling pipes and execution flow.
 * 
 * This function creates multiple child processes to execute a series of commands (represented by `cmds`), handling
 * pipes between commands, and ensuring proper execution flow. For each command, it checks if there are valid arguments,
 * then forks a child process to execute the command using `fork_and_execute`. The function also sets up pipes for inter-process
 * communication using `setup_pipes` and performs necessary cleanup after execution using `cleanup_after_execution`.
 * 
 * @param num_cmds The number of commands to be executed, dictating how many child processes will be created.
 * @param cmds A pointer to the first `t_cmd` structure in the list of commands to be executed.
 * @param ms A pointer to the `t_ms` structure that holds the global execution state, such as exit status.
 * 
 * @return This function does not return; it manages the creation and execution of multiple child processes.
 */

void    make_multiple_childs(int num_cmds, t_cmd *cmds, t_ms *ms)
{
    t_pipe  p;
    t_cmd   *cur;
    int     i;

    i = 0;
    cur = cmds;
    p.num_cmds = num_cmds;
    p.ms = ms;
    //p.pipe_fd = NULL;
    p.last_pid = -1;
    p.cmd_num = 0;
    p.cur_fd = -1;
    p.pids = (pid_t *)malloc((p.num_cmds - 1) * sizeof(pid_t));
    //setup_pipes(&p);
    /*if (!p.pipe_fd)
        return;*/
    while (p.cmd_num < num_cmds && cur)
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
    cleanup_after_execution(&p);
}