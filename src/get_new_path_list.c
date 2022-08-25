/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_new_path_list.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 23:45:50 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/25 01:53:32 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_new_path_list_init_vars(char *path, t_list *file_lst, \
t_get_new_path_list_vars v[1])
{
	v->path = NULL;
	v->path_start = NULL;
	v->path_end = NULL;
	v->wc_str = NULL;
	v->file_lst = NULL;
	v->new_path_lst = NULL;
	v->path = path;
	v->file_lst = file_lst;
}

void	get_new_path_list(char *path, t_list *file_lst, \
t_list **new_path_lst_add)
{
	int							i;
	int							j;
	t_get_new_path_list_vars	v[1];

	i = 0;
	j = 0;
	if (!new_path_lst_add)
		ft_exit(EXIT_FAILURE, "get_new_path_list", "argument check fail");
	get_new_path_list_init_vars(path, file_lst, v);
	if (!v->path || !v->file_lst)
		return ;
	while (v->path[i] && v->path[i] != '*')
		i++;
	while (i > 0 && v->path[i - 1] != '/')
		i--;
	j = i;
	while (v->path[j] && v->path[j] != '/')
		j++;
	fill_new_path_list(i, j, v);
	*new_path_lst_add = v->new_path_lst;
}

void	fill_new_path_list(int i, int j, t_get_new_path_list_vars v[1])
{
	v->path_start = ft_strndup(v->path, i);
	v->path_end = ft_strdup(&(v->path)[j]);
	v->wc_str = ft_strndup(&(v->path)[i], j - i);
	while (v->file_lst)
	{
		if (compatible_name((char *)v->file_lst->content, v->wc_str))
			ft_lstadd_back(&v->new_path_lst, \
			ft_lstnew(ft_strjoin(ft_strjoin(v->path_start, \
			(char *)v->file_lst->content), v->path_end)));
		v->file_lst = v->file_lst->next;
	}
	ft_free(v->path_start);
	ft_free(v->path_end);
	ft_free(v->wc_str);
}
