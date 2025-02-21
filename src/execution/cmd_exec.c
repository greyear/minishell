
#include "../../include/minishell.h"

/*
void execute_cmd(t_shell *core, t_command *command, char *exe_path):
	we choose - if builtin - execute builtin and exit with it's exit code
	if not - exec

void handle_child(t_command *curr_command, int **pipes, t_shell *core, 	char *exe_path):
	checks: redir fds, errors in process, path
	duplicates input and output
	calls execute_cmd, if fails - exits with 1

int	handle_command(t_shell *core, pid_t *children, int **pipes, t_command *command)
	checks access of cmd_name, or calls path finding
	forks child process, calls handle_child. children[index] - array of PID

int	execute_pipeline(t_shell *core, t_command *commands)
	prepares children and pipes
	goes through all the cmds and calls handle_command, terminates children and pipes if error

int	pipeline_execution(t_shell *core, t_command *commands)
	if children are not needed - calls run builtin without fork
	else calls execute_pipeline

void init_pipes(int **pipes, int len)
	sets default values for the array of pipes

int	open_pipes(int **pipes)
	creates pipes (what's the amount?)

void close_pipes(int **pipes)
	closes all pipes

int	prepare_pipes_and_children(t_shell *core, int ***pipes, pid_t **children, int len)
	allocates memory for 1 pipe (pipes are int **) and child (children are pid_t **)
	opens pipe

int	wait_for_children(pid_t *children, int len)
	waitpid(all children processes), returns a return value (last?)

int	finalise_pipes_and_children(int **pipes, pid_t *children, int len)
	calls: closing pipes function, waiting, cleaning

int	dup_input(t_command *command, int **pipes)
	dup2 stdin from infile or pipe end

int	dup_output(t_command *command, int **pipes)
	dup2 stdout to outfile or pipe end

char *find_exe_path(t_shell *core, t_command *command)
	splits paths, joins path + / + cmd, tries access for every
	checks directory, permissions...


*/