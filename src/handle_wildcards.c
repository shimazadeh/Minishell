/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_wildcards.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 15:43:13 by aguillar          #+#    #+#             */
/*   Updated: 2022/09/27 15:32:16 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	replace_node_by_sublist(t_list *node, t_list *sublist)
{
	t_list	*tmp;

	tmp = NULL;
	if (!node || !sublist)
		return ;
	tmp = NULL;
	ft_free(node->content);
	node->content = sublist->content;
	tmp = node->next;
	node->next = sublist->next;
	sublist = ft_lstlast(sublist);
	sublist->next = tmp;
}

void	expand_wc_node(t_list *node)
{
	t_list	*sublist;
	char	*opendir_path;
	char	*path;

	sublist = NULL;
	opendir_path = NULL;
	path = NULL;
	if (!node || !(char *)node->content || !*((char *)node->content))
		ft_exit(EXIT_FAILURE, "expand_wc_node", "argument check fail");
	trim_extra_wc((char *)node->content, &path);
	get_opendir_path(path, &opendir_path);
	get_sublist(&sublist, path, opendir_path);
	if (sublist)
		replace_node_by_sublist(node, sublist);
	ft_free(path);
}

void	handle_wildcards(char ***av_tab_add)
{
	t_list	*av_lst;
	t_list	*node;
	char	**av_tab;

	av_lst = NULL;
	node = NULL;
	av_tab = NULL;
	if (!av_tab_add)
		ft_exit(EXIT_FAILURE, "handle_wildcards", "argument check fail");
	if (!*av_tab_add || !**av_tab_add)
		return ;
	av_tab = *av_tab_add;
	ft_tab_to_lst(av_tab, &av_lst);
	ft_free_tab(av_tab);
	node = av_lst;
	while (node)
	{
		if (ft_strchr((char *)node->content, '*'))
			expand_wc_node(node);
		node = node->next;
	}
	ft_lst_to_tab(&av_tab, &av_lst);
	ft_free_list(av_lst);
	*av_tab_add = av_tab;
}
