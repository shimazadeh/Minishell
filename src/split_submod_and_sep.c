/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_submod_and_sep.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 00:37:38 by aguillar          #+#    #+#             */
/*   Updated: 2022/09/27 15:24:08 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	split_submod_and_sep_init_vars(t_split_submod_and_sep_vars v[1])
{
	v->count = 0;
	v->i = 0;
	v->j = 0;
	v->str = NULL;
}

void	split_submod_and_sep_1(char **str_add, t_list **submod_head, \
t_list **sep_head)
{
	t_split_submod_and_sep_vars	v[1];

	split_submod_and_sep_init_vars(v);
	if (!str_add || !*str_add || !submod_head || !sep_head)
		ft_exit(EXIT_FAILURE, "split_submod_and_sep", "argument check fail");
	rm_outer_par_1(str_add);
	v->str = *str_add;
	split_submod_and_sep_2(submod_head, sep_head, v);
}

void	split_submod_and_sep_2(t_list **submod_head, \
t_list **sep_head, t_split_submod_and_sep_vars v[1])
{
	while (v->str[v->i])
	{
		if (v->str[v->i] == '\"' || v->str[v->i] == '\'')
			v->i += go_to_closing_char(&(v->str[v->i]));
		else if (v->str[v->i] == '(')
			v->count++;
		else if (v->str[v->i] == ')')
			v->count--;
		if (v->count == 0 && (ft_strncmp(&(v->str[v->i]), "||", 2) == 0
				|| ft_strncmp(&(v->str[v->i]), "&&", 2) == 0))
		{
			if (ft_strncmp(&(v->str[v->i]), "||", 2) == 0)
				ft_lstadd_back(sep_head, ft_lstnew(ft_strdup("2")));
			else if (ft_strncmp(&(v->str[v->i]), "&&", 2) == 0)
				ft_lstadd_back(sep_head, ft_lstnew(ft_strdup("1")));
			ft_lstadd_back(submod_head,
				ft_lstnew(ft_strndup(&(v->str[v->j]), v->i - v->j)));
			v->i++;
			v->j = v->i + 1;
		}
		else if (v->count == 0 && !v->str[v->i + 1] && *submod_head)
			ft_lstadd_back(submod_head,
				ft_lstnew(ft_strndup(&(v->str[v->j]), v->i + 1 - v->j)));
		v->i++;
	}
}
