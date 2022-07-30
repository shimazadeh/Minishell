/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Libft_Utils_lst.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shabibol <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/28 01:15:16 by shabibol          #+#    #+#             */
/*   Updated: 2022/05/28 01:15:18 by shabibol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	sc_lstadd_back(t_struct **lst, t_struct *new)
{
	t_struct	*repl;

	if (!new)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	repl = sc_lstlast(*lst);
	repl->next = new;
}

void	sc_lstadd_front(t_struct **lst, t_struct *new)
{
	if (!new)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	new->next = *lst;
	*lst = new;
}

t_struct	*sc_lstlast(t_struct	*lst)
{
	if (!lst)
		return (0);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

int	sc_lstsize(t_struct *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}
