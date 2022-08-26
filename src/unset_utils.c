/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 21:40:04 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/26 23:42:50 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_list_remove_node(t_list **lst_head, t_list *node)
{
	t_list	new_first[1];
	t_list	*prev;
	t_list	*current;

	new_first->next = *lst_head;
	new_first->content = NULL;
	prev = new_first;
	current = *lst_head;
	while (current)
	{
		if (current == node)
		{
			if (current == *lst_head)
				*lst_head = current->next;
			prev->next = current->next;
			ft_free(current->content);
			ft_free(current);
			return (1);
		}
		current = current->next;
		prev = prev->next;
	}
	return (0);
}
