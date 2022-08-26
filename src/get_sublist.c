/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_sublist.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 00:17:28 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/26 22:20:31 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_sublist_init_vars(t_list **sublist, char *path, \
char *opendir_path, t_get_sublist_vars v[1])
{
	v->new_opendir_path = NULL;
	v->new_path = NULL;
	v->opendir_path = NULL;
	v->path = NULL;
	v->wc = NULL;
	v->file_lst = NULL;
	v->new_path_lst = NULL;
	v->sublist = NULL;
	v->opendir_path = opendir_path;
	v->path = path;
	v->sublist = sublist;
}

void	get_sublist(t_list **sublist, char *path, char *opendir_path)
{
	t_get_sublist_vars	v[1];

	if (!sublist || !opendir_path || !*opendir_path || !path || !*path)
		ft_exit(EXIT_FAILURE, "get_sublist", "argument check fail");
	get_sublist_init_vars(sublist, path, opendir_path, v);
	v->wc = ft_strchr(v->path, '*');
	if (!v->wc)
		ft_lstadd_back(v->sublist, ft_lstnew(v->path));
	else
		get_sublist_recursive(v);
}

void	get_sublist_recursive(t_get_sublist_vars v[1])
{
	get_file_list(v->opendir_path, &(v->file_lst));
	if (!v->file_lst)
		return ;
	get_new_path_list(v->path, v->file_lst, &(v->new_path_lst));
	ft_free_list(v->file_lst);
	if (!v->new_path_lst)
		return ;
	while (v->new_path_lst)
	{
		v->new_path = ft_strdup((char *)v->new_path_lst->content);
		get_opendir_path(v->new_path, &v->new_opendir_path);
		get_sublist(v->sublist, v->new_path, v->new_opendir_path);
		ft_free(v->new_opendir_path);
		v->new_path_lst = v->new_path_lst->next;
	}
	ft_free_list(v->new_path_lst);
	ft_free(v->path);
}
