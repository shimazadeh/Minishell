/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_Lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shabibol <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 04:09:48 by shabibol          #+#    #+#             */
/*   Updated: 2022/03/04 22:29:05 by shabibol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*repl;

	if (!new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	repl = ft_lstlast(*lst);
	repl->next = new;
}
