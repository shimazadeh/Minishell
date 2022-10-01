/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 00:21:24 by aguillar          #+#    #+#             */
/*   Updated: 2022/09/30 17:11:15 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	minishell_init_vars(t_minishell_vars *v)
{
	v->last_exit_code = 0;
	v->input = NULL;
	v->prev_input = NULL;
	v->prompt = NULL;
	v->envp_head = NULL;
}

void	minishell(char	**envp)
{
	t_minishell_vars	v[1];
	char				*tmp;

	minishell_init_vars(v);
	tmp = NULL;
	ft_tab_to_lst(envp, &(v->envp_head));
	adjust_env(&(v->envp_head));
	while (1)
	{
		minishell_loop(v, tmp);
	}
}

void	minishell_loop(t_minishell_vars *v, char *tmp)
{
	catch_signals(1);
	get_prompt(&(v->prompt), &(v->envp_head));
	tmp = readline(v->prompt);
	v->input = ft_strdup(tmp);
	free(tmp);
	if (v->input && *(v->input) && !str_is_only_spaces(v->input))
	{
		if (!v->prev_input || \
			ft_strncmp(v->input, v->prev_input, ft_strlen(v->input) + 1))
			add_history(v->input);
		ft_free(v->prev_input);
		v->prev_input = ft_strdup(v->input);
		if (syntax_check(&(v->input), &(v->last_exit_code)))
		{
			handle_ws(&(v->input));
			v->last_exit_code = algorithm(ft_strdup(v->input), \
					&(v->envp_head), v->last_exit_code);
		}
	}
	else if (!v->input && write(1, "exit\n", 5))
		ft_exit(NULL, v->last_exit_code, NULL, NULL);
	ft_free(v->input);
	ft_free(v->prompt);
}
