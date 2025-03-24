#include "../../include/minishell.h"

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

/**
 * @brief Prints a warning message when a here-document is terminated by an end-of-file (EOF) signal.
 * 
 * This function is used when the user interrupts a here-document (a heredoc) with the 
 * end-of-file (EOF) signal (typically Ctrl-D). It prints a warning message indicating 
 * that the here-document was terminated unexpectedly, and shows the delimiter that was 
 * being used to terminate the heredoc.
 * 
 * The format of the warning is:
 * "minishell: warning: here-document at line 1 delimited by end-of-file (wanted `<limiter>`)"
 * where `<limiter>` is replaced with the provided delimiter.
 * 
 * @param limiter The delimiter used to define the end of the here-document.
 */
void	print_heredoc_ctrl_d(char *limiter)
{
	ft_putstr_fd("minishell: warning: here-document at " \
		"line 1 delimited by end-of-file", 2);
	ft_putstr_fd(" (wanted ", 2);
	ft_putstr_fd("`", 2);
	ft_putstr_fd(limiter, 2);
	ft_putstr_fd("')\n", 2);
}

/**
 * @brief Handles the SIGINT signal during heredoc input.
 * 
 * This function is invoked when a SIGINT signal (e.g., from Ctrl+C) is 
 * received during the heredoc process. It outputs a newline to STDERR, 
 * closes the standard input stream to stop further reading, sets a global 
 * signal flag `g_sgnl` to SIGINT, and terminates the program with an exit 
 * status of 130 to indicate the signal was caught.
 * 
 * @param sig The signal number. It checks if the signal is SIGINT (Ctrl+C).
 * 
 * @return None. The function exits the program with a status code of 130 
 *         after handling the signal.
 */
static void	ctrlc_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		write(STDERR_FILENO, "\n", 1);
		close(STDIN_FILENO);
		g_sgnl = SIGINT;
		exit(130);
	}
}

/**
 * @brief Initializes signal handling for SIGINT and SIGQUIT.
 * 
 * This function sets up custom signal handlers for the SIGINT (Ctrl+C) and 
 * SIGQUIT (Ctrl+\) signals by using the `sigaction` system call. It configures 
 * the handlers specified by the `ctrlc` and `ctrlbackslash` function pointers 
 * and ensures that the signals are properly handled during the execution of the program.
 * 
 * - The `SIGINT` signal is typically generated when the user presses Ctrl+C.
 * - The `SIGQUIT` signal is typically generated when the user presses Ctrl+\.
 * 
 * Both signals are configured to restart system calls after being handled (using `SA_RESTART`).
 * 
 * @param ctrlc The custom handler function to handle SIGINT (Ctrl+C).
 * @param ctrlbackslash The custom handler function to handle SIGQUIT (Ctrl+\).
 */
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

/**
 * @brief Configures signal handling based on the specified mode.
 * 
 * This function adjusts the signal handlers based on the given mode. The behavior 
 * of signals such as SIGINT and SIGQUIT can be customized depending on whether 
 * the program is running in a default, interactive, heredoc, or ignore mode.
 * 
 * - **DEFAULT mode**: Restores the default signal handlers for SIGINT and SIGQUIT 
 *   (commonly used for standard program execution, e.g., running `cat`).
 * 
 * - **INTERACTIVE mode**: Configures the signal handler for SIGINT (Ctrl+C) to call 
 *   `ctrlc_interactive` and SIGQUIT to be ignored (used for interactive shell prompts).
 * 
 * - **HEREDOC_MODE**: Configures the signal handler for SIGINT (Ctrl+C) to call 
 *   `ctrlc_heredoc` and SIGQUIT to be ignored (used during heredoc input before 
 *   the delimiter).
 * 
 * - **IGNORE mode**: Ignores both SIGINT and SIGQUIT (possibly used for certain 
 *   input scenarios like the first heredoc).
 * 
 * @param mode The mode to set the signal handlers for. It can be one of the following:
 *        - `DEFAULT`: Restores default signal handling.
 *        - `INTERACTIVE`: Customizes signal handling for interactive prompts.
 *        - `HEREDOC_MODE`: Customizes signal handling during heredoc input.
 *        - `IGNORE`: Ignores both SIGINT and SIGQUIT.
 */
void	signal_mode(t_mode mode)
{
	if (mode == DEFAULT)
		mode_init(SIG_DFL, SIG_DFL);
	if (mode == INTERACTIVE)
		mode_init(ctrlc_interactive, SIG_IGN);
	if (mode == HEREDOC_MODE)
		mode_init(ctrlc_heredoc, SIG_IGN);
	if (mode == IGNORE)
		mode_init(SIG_IGN, SIG_IGN);
}