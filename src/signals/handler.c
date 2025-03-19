
#include "../../include/minishell.h"
#include <signal.h>

/**
 * @brief Initializes terminal settings for signal handling.
 * 
 * This function configures the terminal to disable control character echoing 
 * (such as `Ctrl+C` and `Ctrl+Z`), which is useful for handling terminal signals 
 * in custom shell programs. It uses the `tcgetattr` and `tcsetattr` functions to 
 * modify the terminal settings. If these functions fail, an error is printed.
 * 
 * @return Returns 1 if the terminal settings were successfully initialized, 
 *         or 0 if an error occurred.
 */

int	init_terminal_signals(void)
{
	struct termios	term;

	if (isatty(STDIN_FILENO))
	{
		if (tcgetattr(STDIN_FILENO, &term) == -1)
		{
			perror("tcgetattr failed");
			return (0);
		}
		term.c_lflag &= ~ECHOCTL;
		if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
		{
			perror("tcsetattr failed");
			return (0);
		}
	}
	return (1);
}

/**
 * @brief Handles the SIGINT signal in an interactive shell environment.
 * 
 * This function is triggered when the user presses `Ctrl+C` in the terminal. It:
 * - Prints a newline to the `stderr` stream.
 * - Clears the current line in the readline buffer.
 * - Resets the history with an empty line (using `rl_replace_line` with the `0` flag).
 * - Updates the global signal variable `g_sgnl` with the received signal (`SIGINT`).
 * - Calls `rl_redisplay` to refresh the input prompt.
 * 
 * This function allows a more user-friendly behavior when `Ctrl+C` is pressed, 
 * preventing the shell from exiting and instead clearing the current line and prompt.
 * 
 * @param sig The signal that triggered the handler, in this case, `SIGINT`.
 */

void	ctrlc_interactive(int sig)
{
	if (sig == SIGINT)
	{
		write(STDERR_FILENO, "\n", 1);
		//ioctl(0, TIOCSTI, "\n");
		rl_on_new_line();
		rl_replace_line("", 0); //0 is a flag to unclear the history
		g_sgnl = sig;
		rl_redisplay();
	}
}

void	print_heredoc_ctrl_d(char *limiter)
{
	ft_putstr_fd("minishell: warning: here-document at " \
		"line 1 delimited by end-of-file", 2);
	ft_putstr_fd(" (wanted ", 2);
	ft_putstr_fd("`", 2);
	ft_putstr_fd(limiter, 2);
	ft_putstr_fd("')\n", 2);
}

void	ctrlc_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		write(STDERR_FILENO, "\n", 1);
		close(STDIN_FILENO); // Close stdin to force readline() to return NULL
		//g_sgnl = SIGINT; // Update global signal variable
		exit(130);
	}
}

static void	mode_init(void (*ctrlc)(int), void (*ctrlbackslash)(int))
{
	struct sigaction cc;
	struct sigaction cb;

	ft_memset(&cc, 0, sizeof(cc));
	ft_memset(&cc, 0, sizeof(cb));
	cc.sa_handler = ctrlc;
	cb.sa_handler = ctrlbackslash;
	sigemptyset(&cc.sa_mask);
	sigemptyset(&cb.sa_mask);
	cc.sa_flags = SA_RESTART;
	cb.sa_flags = SA_RESTART;
	sigaction(SIGINT, &cc, NULL);
	sigaction(SIGQUIT, &cb, NULL);
}

void	signal_mode(t_mode mode)
{
	if (mode == DEFAULT) //when we run cat
		mode_init(SIG_DFL, SIG_DFL);
	if (mode == INTERACTIVE) //when we have a prompt
		mode_init(ctrlc_interactive, SIG_IGN);
	if (mode == HEREDOC_MODE) //inside the heredoc before limiter
		mode_init(ctrlc_heredoc, SIG_IGN);
	if (mode == IGNORE) //the 1st (maybe heredoc??)
		mode_init(SIG_IGN, SIG_IGN);
}