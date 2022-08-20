/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_wildcards.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 15:43:13 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/20 01:20:05 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_list(t_list *list)
{
	if (!list)
	{
		dprintf(2, "list is NULL\n");
		return ;
	}
	while (list)
	{
		dprintf(2, "%s\n", (char *)list->content);
		list = list->next;
	}
}

void	print_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
	{
		dprintf(2, "tab is NULL\n");
		return ;
	}
	while (tab[i])
	{
		dprintf(2, "%s\n", tab[i]);
		i++;
	}
}

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

int	next_char_index(char *str, char c)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] && str[i] != c)
		i++;
	if (!str[i])
		return (0);
	return (i);
}

int	compatible_name(char *file, char *wc)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	k = 0;
	if (!file || !wc)
		return (0);
	if (*wc != '*' && ft_strncmp(file, wc, next_char_index(wc, '*')))
		return (0);
	j = i + 1;
	while (file[i])
	{
		k =  next_char_index(wc, '*');
		if (wc[j] && ft_strncmp(&file[i], &wc[j], k))
		{
			j += k + 1;
			i += k;
		}
		i++;
	}
	if (!wc[j])
		return (1);
	return(0);
}

t_list	*get_new_path(char *path, char **file_tab)
{
	char	*content;
	char	*wc_str;
	int		i;
	int		j;
	t_list	*new;
	t_list	*new_path_lst;

	content = NULL;
	wc_str = NULL;
	i = 0;
	j = 0;
	new = NULL;
	new_path_lst = NULL;
	if (!path || !file_tab || !*file_tab)
		return (NULL);
	while (path[i] && path[i] != '*')
		i++;
	while (i > 0 && path[i - 1] != '/')
		i--;
	while (path[i + j] && path[i + j] != '/')
		j++;
	wc_str = ft_strndup(&path[i], j);
	if (!wc_str)
		ft_exit(errno, NULL);
	path[i] = '\0';
	i = 0;
	while (file_tab[i])
	{
		if (compatible_name(file_tab[i], wc_str))
		{
			content = ft_strjoin(path, file_tab[i]);
			if (!content)
				ft_exit(errno, NULL);
			new = ft_lstnew(content);
			if (!new)
				ft_exit(errno, NULL);
			ft_lstadd_back(&new_path_lst, new);
		}
		i++;
	}
	return (new_path_lst);
}

int	read_dir_content(struct dirent **dir_content_add, DIR *stream)
{
	struct dirent	*dir_content;

	if (!dir_content_add)
		ft_exit(EXIT_FAILURE, NULL);
	if (!*dir_content_add || !stream)
		return (0);
	dir_content = NULL;
	dir_content = readdir(stream);
	if (!dir_content)
		return (0);
	*dir_content_add = dir_content;
	return (1);
}

char	**get_file_tab(char *path)
{
	char			*dir_path;
	DIR				*stream;
	struct dirent	*dir_content;
	char			*file_str;
	char			**file_tab;
	char			*tmp;
	int				i;

	dir_path = NULL;
	stream = NULL;
	dir_content = NULL;
	file_str = NULL;
	file_tab = NULL;
	tmp = NULL;
	i = 0;
	if (!path)
		return (NULL);
	dir_path = ft_strdup(path);
	if (!dir_path)
		ft_exit(errno, NULL);
	while (dir_path[i] && dir_path[i] != '*')
		i++;
	while (i > 1 && dir_path[i - 1] != '/')
		i--;
	dprintf(2, "IN get_file_tab path %s, i %d\n", path, i);
// pb : only works if abs path
	dir_path[i] = '\0';
	stream = opendir(dir_path);
	if (!stream)
		return (NULL);
	while (read_dir_content(&dir_content, stream))
	{
		tmp = file_str;
		file_str = ft_strjoin(tmp, " ");
		ft_free(tmp);
		file_str = ft_strjoin(tmp, dir_content->d_name);
		ft_free(tmp);
	}
	file_tab = ft_split(file_str, ' ');
	print_tab(file_tab);
	if (!file_tab)
		ft_exit(errno, NULL);
	free(file_str);
	if (!*file_tab)
		return (NULL);
	return (file_tab);
}

void	get_sublist(t_list **sublist, char *path)
{
	char	*wc;
	char	**file_tab;
	t_list	*new_path_lst;
	char	*new_path;
	t_list	*new;

	wc = NULL;
	file_tab = NULL;
	new_path_lst = NULL;
	new_path = NULL;
	new = NULL;
	if (!sublist)
		ft_exit(EXIT_FAILURE, NULL);
	if (!path)
		return ;
	wc = ft_strchr(path, '*');
	if (!wc)
	{
		new = ft_lstnew(path);
		if (!new)
			ft_exit(errno, NULL);
		ft_lstadd_back(sublist, new);
	}
	else
	{
		file_tab = get_file_tab(path);
		if (!file_tab)
			return ;
		new_path_lst = get_new_path(path, file_tab);
		ft_free_tab(file_tab);
		if (!new_path_lst)
			return ;
		while(new_path_lst)
		{
			new_path = ft_strdup((char *)new_path_lst->content);
			if (!new_path)
				ft_exit(errno, NULL);
			get_sublist(sublist, new_path);
			ft_free(new_path);
			new_path_lst = new_path_lst->next;
		}
		ft_free_list(new_path_lst);
	}
	ft_free(path);
}

char	*trim_extra_wc(char	*str)
{
	int		i;
	int		j;
	char	*path;

	i = 0;
	j = 0;
	path = NULL;
	if (!str)
		return (NULL);
	while (str[i])
	{
		if (i > 0 && !ft_strncmp(&str[i - 1], "**", 2))
			i++;
		else
		{
			i++;
			j++;
		}
	}
	path = ft_alloc(sizeof(char) * (j + 1));
	i = 0;
	j = 0;
	while (str[i])
	{
		if (i > 0 && !ft_strncmp(&str[i - 1], "**", 2))
			i++;
		else
		{
			path[j] = str[i];
			i++;
			j++;
		}
	}
	path[j] = '\0';
	return (path);
}

void	expand_wc_node(t_list *node)
{
	t_list	*sublist;
	char	*path;

	sublist = NULL;
	path = NULL;
	if (!node)
		return ;
	path = trim_extra_wc((char *)node->content);
	get_sublist(&sublist, path);
	print_list(sublist);
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
		ft_exit(EXIT_FAILURE, NULL);
	if (!*av_tab_add || !***av_tab_add)
		return ; 
	av_tab = *av_tab_add;
	tab_to_list(av_tab, &av_lst);
	node = av_lst;
	while (node)
	{
		if (ft_strchr((char *)node->content, '*'))
			expand_wc_node(node);
		node = node->next;
	}
	ft_free_tab(av_tab);
	av_tab = NULL;
	lst_to_tab(&av_tab, &av_lst);
	*av_tab_add = av_tab;
}
