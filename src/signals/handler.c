
#include "../../include/minishell.h"
#include <signal.h>


/*#ifndef SA_RESTART
# define SA_RESTART 0
#endif*/

void	ctrlc_interactive(int sig)
{
	if (sig == SIGINT)
	{
		//printf("\n");
		write(STDERR_FILENO, "\n", 1);
		//ioctl(0, TIOCSTI, "\n");
		rl_on_new_line();
		rl_replace_line("", 0); //0 is a flag to unclear the history
		g_sgnl = sig;
		rl_redisplay();
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
		mode_init(SIG_DFL, SIG_IGN);
	if (mode == IGNORE) //the 1st (maybe heredoc??)
		mode_init(SIG_IGN, SIG_IGN);
}
