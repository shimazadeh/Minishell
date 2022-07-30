/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shabibol <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 04:10:24 by shabibol          #+#    #+#             */
/*   Updated: 2022/03/02 11:18:27 by shabibol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*place;

	if (!*lst)
		return ;
	while (*lst)
	{
		place = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = place;
	}
}
