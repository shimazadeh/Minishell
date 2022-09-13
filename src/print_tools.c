/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 00:23:10 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/25 00:23:29 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_list(t_list *list)
{
	if (!list)
	{
		ft_dprintf(2, "list is NULL\n");
		return ;
	}
	while (list)
	{
		ft_dprintf(2, "%s\n", (char *)list->content);
		list = list->next;
	}
}

void	print_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
	{
		ft_dprintf(2, "tab is NULL\n");
		return ;
	}
	while (tab[i])
	{
		ft_dprintf(2, "%s\n", tab[i]);
		i++;
	}
}
