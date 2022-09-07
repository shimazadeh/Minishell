/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 00:21:24 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/26 22:54:00 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	minishell_init_vars(t_minishell_vars *v)
{
	v->last_exit_code = 0;
	v->input = NULL;
	v->prompt = NULL;
	v->envp_head = NULL;
}

void	handle_signal(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (signum == SIGQUIT)
		return ;
	return ;
}

void	minishell(char	**envp)
{
	t_minishell_vars	v[1];

	minishell_init_vars(v);
	ft_tab_to_lst(envp, &(v->envp_head));
	while (1)
	{
		signal(SIGINT, handle_signal);
		signal(SIGQUIT, handle_signal);
		get_prompt(&(v->prompt), &(v->envp_head));
		v->input = readline(v->prompt);
		if (v->input && *(v->input))
		{
			if (!str_is_only_spaces(v->input))
				add_history(v->input);
			if (even_par_nbr(v->input) && no_unclosed_quote(v->input))
			{
				handle_ws(&(v->input));
				v->last_exit_code = algorithm(ft_strdup(v->input),
						&(v->envp_head), v->last_exit_code);
			}
		}
		else if (!v->input)
		{
			write(1, "exit", 4);
			ft_exit(0, NULL, NULL);
		}
		ft_free(v->input);
		ft_free(v->prompt);
	}
}
