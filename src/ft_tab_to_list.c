/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tab_to_list.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 12:43:03 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/24 13:31:10 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_tab_to_list(char **tab, t_list **lst_head)
{
	int		i;
	t_list	*new;
	char	*str;

	i = 0;
	if (!lst_head)
		ft_exit(EXIT_FAILURE, "Exited in function: tab_to_list\nExit due to: argument check fail\n");
	if (!tab || !tab[0])
		return ;
	while (tab[i])
	{
		str = ft_strdup(tab[i]);
		new = ft_lstnew(str);
		ft_lstadd_back(lst_head, new);
		i++;
	}
}
