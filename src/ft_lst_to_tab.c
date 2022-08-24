/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_to_tab.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 13:34:26 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/25 01:51:30 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_lst_to_tab(char ***tab_add, t_list **lst_head)
{
	char	**tab;
	int		size;
	int		i;
	t_list	*node;

	if (!lst_head || !tab_add)
		ft_exit(EXIT_FAILURE, "lst_to_tab", "argument check fail");
	node = *lst_head;
	size = ft_lstsize(node);
	tab = ft_alloc(sizeof(char *) * (size + 1));
	i = 0;
	while (i < size)
	{
		tab[i] = ft_strdup((char *)node->content);
		node = node->next;
		i++;
	}
	tab[i] = NULL;
	*tab_add = tab;
}
