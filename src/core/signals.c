/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:08 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

#include <signal.h>
#include <termios.h>

static void	sigint_handler(int sig)
{
	(void)sig;
	set_signal_number(SIGINT);
	if (is_interactive_mode())
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		set_exit_status(130);
	}
	else
		set_exit_status(130);
}

static void	sigquit_handler(int sig)
{
	(void)sig;
	if (is_interactive_mode())
		return ;
	set_signal_number(SIGQUIT);
	write(STDOUT_FILENO, "Quit (core dumped)\n", 18);
	set_exit_status(131);
}

static void	sigint_handler_heredoc(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	set_exit_status(130);
	set_signal_number(SIGINT);
	rl_replace_line("", 0);
	rl_done = 1;
}

void	set_signal(void)
{
	if (isatty(STDIN_FILENO))
		set_termios();
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
	signal(SIGPIPE, SIG_IGN);
}

void	set_heredoc_signal(void)
{
	struct sigaction	sa;

	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = sigint_handler_heredoc;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
}
