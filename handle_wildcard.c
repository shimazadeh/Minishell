/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 15:43:13 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/19 19:19:17 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_wc_node(t_list *node)
{
	0. split path and file
	1. get sublist (path + file)
	recursively lauch getpath
	which recusrively lauch get file
	if (sublist)
		replace_node_by_sublist(node, sublist);
}

void	handle_wildcards(char ***av_tab_add)
{
	t_list	*av_lst;
	t_list	*node;
	char	*av_tab;

	av_lst = NULL;
	av_tab = *av_tab_add;
	tab_to_list(av_tab, &av_lst);
	node = av_lst;
	while (node)
	{
		if (ft_strchr((char *)node->content, '*'))
			expand_wc_node(node);
		node = node->next;
	}
	free_tab(av_tab);
	av_tab = NULL;
	lst_to_tab(&av_tab, &env)
}
