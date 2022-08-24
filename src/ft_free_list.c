/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 13:22:55 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/24 13:23:06 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
