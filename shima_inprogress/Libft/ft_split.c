/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shabibol <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 05:03:39 by shabibol          #+#    #+#             */
/*   Updated: 2022/03/04 22:35:49 by shabibol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_size_word(char const *str, char charset)
{
	int	nbr;
	int	detect;

	nbr = 0;
	detect = 1;
	if (!str)
		return (0);
	if (*str && !charset)
		return (nbr + 1);
	while (*str)
	{
		if (*str != charset)
		{
			if (detect == 1)
				nbr++;
			detect = 0;
		}
		else
			detect = 1;
		str++;
	}
	return (nbr);
}

int	ft_tab_size(char const *str, char charset)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != charset)
		i++;
	return (i);
}

char	**ft_free(char **dst, int i)
{
	while (i > 0)
	{
		i--;
		free((void *)dst[i]);
	}
	free (dst);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;
	int		i;
	int		j;
	int		size;

	size = ft_size_word(s, c);
	tab = (char **)malloc (sizeof(char *) * (size + 1));
	if (!s || tab == 0)
		return (NULL);
	i = 0;
	while (s && i < size)
	{
		j = 0;
		while (*s && *s == c)
			s++;
		tab[i] = (char *) malloc(sizeof(char) * ft_tab_size(s, c) + 1);
		if (tab[i] == 0)
			return (ft_free(tab, i));
		while (*s && *s != c)
			tab[i][j++] = *s++;
		tab[i][j] = '\0';
		i++;
	}
	tab[i] = NULL;
	return (tab);
}
