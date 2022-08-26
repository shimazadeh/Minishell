/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_sc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 21:20:07 by shabibol          #+#    #+#             */
/*   Updated: 2022/08/26 22:03:28 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_sc(t_struct *lst)
{
	t_struct	*tmp;

	if (!lst)
		return ;
	while (lst)
	{
		ft_free_tab(lst->cmd);
		ft_free_tab(lst->infiles);
		ft_free_tab(lst->outfiles);
		ft_free(lst->str);
		ft_free(lst->outfile_modes);
		tmp = lst;
		lst = lst->next;
		ft_free(tmp);
	}
	return ;
}
