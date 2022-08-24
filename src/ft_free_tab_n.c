/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_tab_n.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 13:24:18 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/24 13:24:44 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_tab_n(char **tab, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		if (tab[i])
			ft_free(tab[i]);
		i++;
	}
	ft_free(tab);
}
