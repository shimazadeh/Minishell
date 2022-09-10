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

void	handle_signal(int signum)
{
	if (signum == SIGINT)
	{
		g_var->sig_flag = 1;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (signum == SIGQUIT)
		ft_dprintf(1, "\b\b  \b\b");
	return ;
}

void	handle_signal_pipe(int signum)
{
	if (signum == SIGINT)
	{
		close (0);
		g_var->sig_flag = 1;
	}
	else if (signum == SIGQUIT)
		ft_dprintf(1, "\b\b  \b\b");
	return ;
}
