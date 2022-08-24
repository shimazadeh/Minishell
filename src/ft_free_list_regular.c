/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_list_regular.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 19:04:38 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/24 13:27:06 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_list_regular(t_list *list)
{
	t_list	*tmp;

	while (list)
	{
		tmp = list->next;
		free(list->content);
		free(list);
		list = tmp;
	}
}
