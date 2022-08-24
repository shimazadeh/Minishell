/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 18:53:45 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/24 13:38:24 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_alloc(int size)
{
	void	*alloc;
	t_list	*new;

	alloc = malloc(size);
	if (!alloc)
		ft_exit(errno, "Exited in function: ft_alloc\nExit due to: malloc fail\n");
	new = ft_lstnew_regular(alloc);
	if (!new)
		ft_exit(errno, "Exited in function: ft_alloc\nExit due to: malloc fail\n");
	ft_lstadd_back(&g_alloc_lst, new);
	return (alloc);
}

int	ft_free(void *to_free)
{
	t_list	new_first[1];
	t_list	*prev;
	t_list	*current;

	if (!to_free)
		return (0);
	new_first->next = g_alloc_lst;
	new_first->content = NULL;
	prev = new_first;
	current = g_alloc_lst;
	while (current)
	{
		if (current->content == to_free)
		{
			if (current == g_alloc_lst)
				g_alloc_lst = current->next;
			prev->next = current->next;
			free(current->content);
			free(current);
			current = prev->next;
			return (1);
		}
		current = current->next;
		prev = prev->next;
	}
	return (0);
}
