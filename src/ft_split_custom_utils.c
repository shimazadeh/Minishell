/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_custom_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 23:19:22 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/24 23:22:25 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_wc_custom(char *str, char c)
{
	int	i;
	int	wc;

	i = 0;
	wc = 0;
	while (str[i])
	{
		while (str[i] && (str[i] == c))
			i++;
		if (str[i] && (str[i] != c))
		{
			wc++;
			while (str[i] && (str[i] != c))
			{
				if (str[i] && (str[i] == '\"' || str[i] == '\''))
					i += go_to_closing_char((char *)&str[i]) + 1;
				else
					i++;
			}
		}
	}
	return (wc);
}

int	ft_wl_custom(char *str, char c, int flag)
{
	int	i;
	int	wl;

	i = 0;
	wl = 0;
	while (str[i] && (str[i] == c))
		i++;
	while (str[i] && (str[i] != c))
		ft_get_wl(str, &i, &wl, flag);
	return (wl);
}

void	ft_get_wl(char *str, int *i_add, int *wl_add, int flag)
{
	int	i;
	int	gtcc;
	int	wl;

	i = *i_add;
	gtcc = 0;
	wl = *wl_add;
	if (str[i] && (str[i] == '\"' || str[i] == '\''))
	{
		gtcc = go_to_closing_char((char *)&str[i]);
		i += gtcc + 1;
		wl += gtcc + 1;
		if (flag)
			wl -= 2;
	}
	else
	{
		wl++;
		i++;
	}
	*i_add = i;
	*wl_add = wl;
}

void	ft_get_tab(t_split_custom_vars v[1], t_ft_fill_tab_custom_vars w[1])
{
	if (v->str[w->k] && (v->str[w->k] == '\"' || v->str[w->k] == '\''))
	{
		w->gtcc = go_to_closing_char((char *)&v->str[w->k]);
		if (v->flag)
			w->k++;
		while ((w->gtcc - v->flag * 2) >= 0)
		{
			v->tab[w->i][w->j++] = v->str[w->k++];
			w->gtcc--;
		}
		if (v->flag)
			w->k++;
	}
	else
		v->tab[w->i][w->j++] = v->str[w->k++];
}
