/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_custom.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 16:48:29 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/20 19:09:03 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_wc_custom(char *str, char c)
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
					i += go_to_closing_char((char *)&str[i]);
				i++;
			}
		}
	}
	return (wc);
}

static int	ft_wl_custom(char *str, char c)
{
	int	i;
	int	wl;
	int	gtcc;

	i = 0;
	wl = 0;
	gtcc = 0;
	while (str[i] && (str[i] == c))
		i++;
	while (str[i] && (str[i] != c))
	{
		if (str[i] && (str[i] == '\"' || str[i] == '\''))
		{
			gtcc = go_to_closing_char((char *)&str[i]);
			i += gtcc + 1;
			wl += gtcc - 1;
		}
		wl++;
		i++;
	}
	return (wl);
}

static int	ft_fill_tab_custom(char **tab, int wc, const char *str, char c)
{
	int	i;
	int	j;
	int	k;
	int	gtcc;

	i = 0;
	k = 0;
	gtcc = 0;
	while (i < wc)
	{
		j = 0;
		tab[i] = ft_alloc(sizeof(char) * (ft_wl_custom((char *)&str[k], c) + 1));
		if (!tab[i])
		{
			ft_free_tab(tab);
			return (0);
		}
		while (str[k] && (str[k] == c))
			k++;
		while (str[k] && (str[k] != c))
		{
			if (str[k] && (str[k] == '\"' || str[k] == '\''))
			{
				gtcc = go_to_closing_char((char *)&str[k]);
				k++;
				while (gtcc - 2)
				{
					tab[i][j++] = str[k++];
					gtcc--;
				}
				k++;
			}
			tab[i][j++] = str[k++];
		}
		tab[i][j] = '\0';
		i++;
	}
	tab[i] = 0;
	return (1);
}

char	**ft_split_custom(const char *str, char c)
{
	char	**tab;
	int		wc;

	if (!str)
		return (0);
	wc = ft_wc_custom((char *)str, c);
	tab = ft_alloc(sizeof(char *) * (wc + 1));
	if (!tab)
		return (0);
	if (!ft_fill_tab_custom(tab, wc, str, c))
		return (0);
	return (tab);
}


// int	main(int ac, char **av)
// {
// 	char **tab;
// 	int		i;

// 	i = 0;
// 	tab = ft_split_custom("cat \"<infile1 \"| ls |\"cat |cat|ls >outfile", '|');
// 	while(tab[i])
// 	{
// 		printf(">>>%s<<<\n", tab[i]);
// 		i++;
// 	}
// 	return (0);
// }
