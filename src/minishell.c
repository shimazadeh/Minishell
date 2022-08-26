/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 00:21:24 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/26 22:15:21 by aguillar         ###   ########.fr       */
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

void	minishell(char	**envp)
{
	t_minishell_vars	v[1];

	minishell_init_vars(v);
	ft_tab_to_lst(envp, &(v->envp_head));
	while (1)
	{
		get_prompt(&(v->prompt), &(v->envp_head));
		v->input = readline(v->prompt);
		if (v->input && *(v->input))
		{
			if (!str_is_only_spaces(input))
				add_history(v->input);
			if (even_par_nbr(v->input) && no_unclosed_quote(v->input))
			{
				handle_ws(&(v->input));
				v->last_exit_code = algorithm(ft_strdup(v->input),
						&(v->envp_head), v->last_exit_code);
			}
		}
		ft_free(v->input);
		ft_free(v->prompt);
	}
}
