/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 18:53:45 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/20 22:57:51 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_alloc(int size)
{
	void	*alloc;
	t_list	*new;

	alloc = malloc(size);
	if (!alloc)
		ft_exit(EXIT_FAILURE, NULL);
	new = ft_lstnew_regular(alloc);
	if (!new)
		ft_exit(EXIT_FAILURE, NULL);
	ft_lstadd_back(&alloc_lst, new);
	return (alloc);
}

int	ft_free(void *to_free)
{
	t_list	new_first[1];
	t_list	*prev;
	t_list	*current;

	if (!to_free)
		return (0);
	new_first->next = alloc_lst;
	new_first->content = NULL;
	prev = new_first;
	current = alloc_lst;
	while (current)
	{
		if (current->content == to_free)
		{
			if (current == alloc_lst)
				alloc_lst = current->next;
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

void	ft_free_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
	{
		ft_free(tab[i]);
		i++;
	}
	ft_free(tab);
}

void	ft_free_list(t_list *list)
{
	t_list	*tmp;

	while (list)
	{
		tmp = list->next;
		ft_free(list->content);
		ft_free(list);
		list = tmp;
	}
}
