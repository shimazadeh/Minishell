/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 15:43:13 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/19 19:33:48 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	replace_node_by_sublist(t_list *node, t_list *sublist)
{
	t_list	*tmp;

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

void	split_path_and_file(t_list *node, char **path_add, char **file_add)
{
	char	*path;
	char	*file;

	path = NULL;
	file = NULL;
	str = ft_strdup((char *)node->content);
	if (!str)
		ft_exit(errno, NULL);
	i = ft_strlen(str);
	if (i)
		i--;
	while (i > 0 && str[i - 1] != '/')
		i--;
	file = ft_strdup(&str[i]);
	if (!file)
		ft_exit(errno, NULL);
	str[i] = '\0';
	path = ft_strdup(str);
	if (!path)
		ft_exit(errno, NULL);
	ft_free(str);
	*path_add = path;
	*file_add = file;
}

void	expand_wc_node(t_list *node)
{
	t_list	*sublist;
	char	*file;
	char	*path;
	char	*str;
	int		i;

	sublist = NULL;
	file = NULL;
	path = NULL;
	str = NULL;
	i = 0;
	if (!node)
		ft_exit(EXIT_FAILURE, NULL);
	split_path_and_file(&path, &file, node);
	get_sublist(&sublist, path, file);
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
