/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 17:21:24 by shabibol          #+#    #+#             */
/*   Updated: 2022/10/04 14:10:50 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fill_var_expand_tab(char ***tab_add, char *str)
{
	char	**tab;
	int		i;
	int		j;
	int		index;

	tab = *tab_add;
	i = 0;
	j = 0;
	index = 0;
	while (str[i])
	{
		j = i;
		if (str[i] == '\'' || str[i] == '\"')
			i += go_to_closing_char(&str[i]) + 1;
		else
		{
			while (str[i] && str[i] != '\'' && str[i] != '\"')
				i++;
		}
		tab[index] = ft_strdup_range(str, j, i);
		index++;
	}
}

char	**split_variable_expansion(char *str)
{
	int		count;
	char	**tab;

	count = sizeof(char *) * (var_expand_size(str) + 1);
	tab = ft_alloc(count);
	ft_bzero(tab, count);
	fill_var_expand_tab(&tab, str);
	return (tab);
}

char	*variable_expansion(char *str, t_list **envp, int last_ec, int flag)
{
	int		i;
	char	**tab;
	char	*new_str;
	int		index;

	tab = split_variable_expansion(str);
	i = -1;
	while (tab[++i])
	{
		if (tab[i][0] != '\'' || (tab[i][0] == '\'' && flag))
		{
			index = 0;
			while (tab[i][index])
			{
				if (tab[i][index] == '$' && tab[i][index + 1] == '?')
					tab[i] = expand_last_ec(tab[i], &index, last_ec);
				else if (tab[i][index] == '$')
					tab[i] = expand_variable(tab[i], &index, envp, flag);
				else
					index++;
			}
		}
	}
	new_str = ft_jointab_1(tab);
	return (ft_free(str), ft_free_tab(tab), new_str);
}

char	*ft_jointab_1(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		if (tab[i][ft_strlen(tab[i]) - 1] == '$' && tab[i + 1] \
			&& (tab[i + 1][0] == '\"' || tab[i + 1][0] == '\''))
			tab[i][ft_strlen(tab[i]) - 1] = '\0';
		i++;
	}
	return (ft_jointab_2(tab));
}

char	*ft_jointab_2(char **tab)
{
	int		size;
	int		i;
	int		j;
	int		k;
	char	*str;

	size = 0;
	i = 0;
	while (tab[i])
	{
		size += ft_strlen(tab[i]);
		i++;
	}
	str = ft_alloc(sizeof(char) * (size + 1));
	str[size] = '\0';
	k = 0;
	i = 0;
	while (tab[i])
	{
		j = 0;
		while (tab[i][j])
			str[k++] = tab[i][j++];
		i++;
	}
	return (str);
}
