/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 00:21:24 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/24 00:31:37 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_minishell_vars
{
	int				last_exit_code;
	char			*input;
	char			*prompt;
	t_list			*envp_head;
}				t_minishell_vars;

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
	tab_to_list(envp, &(v->envp_head));
	while (1)
	{
		get_prompt(&(v->prompt), &(v->envp_head));
		v->input = readline(v->prompt);
		if (v->input && *(v->input))
		{
			if (even_par_nbr(v->input))
			{
				add_history(v->input);
				v->last_exit_code = algorithm(ft_strdupv(v->input),
						&(v->envp_head), v->last_exit_code);
			}
			else
				ft_dprintf(2,
					"Error\nInput contains odd number of parentheses!\n");
		}
		ft_free(v->input);
		ft_free(v->prompt);
	}
}
