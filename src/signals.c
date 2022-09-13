/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 12:43:22 by shabibol          #+#    #+#             */
/*   Updated: 2022/09/08 20:49:17 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	catch_signals(int flag)
{
	static struct sigaction	_sigact;
	static struct sigaction	_sigact_pipe;
	static struct sigaction	_sigact_heredoc;

	memset(&_sigact, 0, sizeof(_sigact));
	_sigact.sa_sigaction = sig_handler;
	_sigact.sa_flags = SA_SIGINFO;
	memset(&_sigact_pipe, 0, sizeof(_sigact_pipe));
	_sigact_pipe.sa_sigaction = sig_handler_pipe;
	_sigact_pipe.sa_flags = SA_SIGINFO;
	memset(&_sigact_heredoc, 0, sizeof(_sigact));
	_sigact_heredoc.sa_sigaction = sig_handler_heredoc;
	_sigact_heredoc.sa_flags = SA_SIGINFO;
	sigaction(SIGQUIT, &_sigact, NULL);
	if (flag == 0)
		sigaction(SIGINT, &_sigact_pipe, NULL);
	else if (flag == 1)
		sigaction(SIGINT, &_sigact, NULL);
	else if (flag == 2)
		sigaction(SIGINT, &_sigact_heredoc, NULL);
}

void	sig_handler(int signum, siginfo_t *info, void *ptr)
{
	(void)ptr;
	(void)info;
	if (signum == SIGINT)
	{
		g_var->sig_flag = 1;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (signum == SIGQUIT)
		write(1, "\b\b  \b\b", 6);
}

void	sig_handler_pipe(int signum, siginfo_t *info, void *ptr)
{
	(void)ptr;
	(void)info;
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		g_var->sig_flag = 1;
	}
}

void	sig_handler_heredoc(int signum, siginfo_t *info, void *ptr)
{
	(void)ptr;
	(void)info;
	if (signum == SIGINT)
	{
		close (0);
		write(1, "\n", 1);
		g_var->sig_flag = 1;
	}
}
