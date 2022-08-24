/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_custom.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 16:48:29 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/25 01:40:35 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_split_custom_init_vars(const char *str, char c, int flag, \
t_split_custom_vars v[1])
{
	v->c = 0;
	v->flag = 0;
	v->wc = 0;
	v->str = NULL;
	v->tab = NULL;
	v->c = c;
	v->flag = flag;
	v->str = str;
}

char	**ft_split_custom(const char *str, char c, int flag)
{
	t_split_custom_vars	v[1];

	ft_split_custom_init_vars(str, c, flag, v);
	if (!str)
		return (0);
	v->wc = ft_wc_custom((char *)v->str, v->c);
	v->tab = ft_alloc(sizeof(char *) * (v->wc + 1));
	if (!v->tab)
		return (0);
	if (!ft_fill_tab_custom(v))
		return (0);
	return (v->tab);
}

void	ft_fill_tab_custom_init_vars(t_ft_fill_tab_custom_vars w[1])
{
	w->gtcc = 0;
	w->i = 0;
	w->j = 0;
	w->k = 0;
}

int	ft_fill_tab_custom(t_split_custom_vars v[1])
{
	t_ft_fill_tab_custom_vars	w[1];

	ft_fill_tab_custom_init_vars(w);
	while (w->i < v->wc)
	{
		w->j = 0;
		v->tab[w->i] = ft_alloc(sizeof(char)
				* (ft_wl_custom((char *)&(v->str)[w->k], v->c, v->flag) + 1));
		while (v->str[w->k] && (v->str[w->k] == v->c))
			w->k++;
		while (v->str[w->k] && (v->str[w->k] != v->c))
			ft_get_tab(v, w);
		v->tab[w->i][w->j] = '\0';
		w->i++;
	}
	v->tab[w->i] = 0;
	return (1);
}
