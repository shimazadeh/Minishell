/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_wildcards.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 15:43:13 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/24 13:45:38 by aguillar         ###   ########.fr       */
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

///////////////////////////////////////////////////////////////////////////////////////

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
	if (!str || !*str)
		return (0);
	while (str[i] && str[i] != c)
		i++;
	if (!str[i])
		return (i + 1);
	return (i);
}

int	compatible_name(char *file, char *wc)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 1;
	k = 0;
	if (!file || !wc)
		return (0);
	if (file[0] == '.' && wc[0] != '.')
		return (0);
	if (wc[0] != '*')
	{
		k = next_char_index(wc, '*');
		if (ft_strncmp(wc, file, k))
			return (0);
		else
		{
			i = k;
			j = k + 1;
		}
	}
	while (file[i])
	{
		k = next_char_index(&wc[j], '*');
		if (wc[j] && !ft_strncmp(&file[i], &wc[j], k))
		{
			j += k - 1;
			i += k - 1;
			if (wc[j])
				j += 2;
			if (file[i])
				i++;
		}
		else
			i++;
	}
	if (!wc[j])
		return (1);
	return(0);
}

void	get_new_path_list(char *path, t_list *file_lst, t_list **new_path_lst_add)
{
	char	*content;
	char	*path_start;
	char	*path_end;
	char	*tmp;
	char	*wc_str;
	int		i;
	int		j;
	t_list	*new;
	t_list	*new_path_lst;

	content = NULL;
	path_start = NULL;
	path_end = NULL;
	tmp = NULL;
	wc_str = NULL;
	i = 0;
	j = 0;
	new = NULL;
	new_path_lst = NULL;
	if (!new_path_lst_add)
		ft_exit(EXIT_FAILURE, "Exited in function: get_new_path_list\nExit due to: argument check fail\n");
	if (!path || !file_lst)
		return ;
	while (path[i] && path[i] != '*')
		i++;
	while (i > 0 && path[i - 1] != '/')
		i--;
	j = i;
	while (path[j] && path[j] != '/')
		j++;
	path_start = ft_strndup(path, i);
	path_end = ft_strdup(&path[j]);
	wc_str = ft_strndup(&path[i], j - i);
	while (file_lst)
	{
		if (compatible_name((char *)file_lst->content, wc_str))
		{
			content = ft_strjoin(path_start, (char *)file_lst->content);
			tmp = content;
			content = ft_strjoin(tmp, path_end);
			ft_free(tmp);
			new = ft_lstnew(content);
			ft_lstadd_back(&new_path_lst, new);
		}
		file_lst = file_lst->next;
	}
	ft_free(path_start);
	ft_free(path_end);
	ft_free(wc_str);
	*new_path_lst_add = new_path_lst;
}

int	read_dir_content(struct dirent **dir_content_add, DIR *stream)
{
	struct dirent	*dir_content;

	if (!dir_content_add)
		ft_exit(EXIT_FAILURE, "Exited in function: read_dir_content\nExit due to: argument check fail\n");
	if (!stream)
		return (0);
	dir_content = NULL;
	dir_content = readdir(stream);
	if (!dir_content)
		return (0);
	*dir_content_add = dir_content;
	return (1);
}

void	get_file_list(char *opendir_path, t_list **file_lst_add)
{
	DIR				*stream;
	struct dirent	*dir_content;
	t_list			*file_lst;
	char			*tmp;

	stream = NULL;
	dir_content = NULL;
	file_lst = NULL;
	if (!opendir_path || !*opendir_path || !file_lst_add)
		ft_exit(EXIT_FAILURE, "Exited in function: get_file_tab\nExit due to: argument check fail\n");
	stream = opendir(opendir_path);
	if (!stream)
		return ;
	while (read_dir_content(&dir_content, stream))
	{
		tmp = ft_strdup((char *)dir_content->d_name);
		ft_lstadd_back(&file_lst, ft_lstnew(tmp));
	}
	closedir(stream);
	if (!file_lst)
		return;
	*file_lst_add = file_lst; 
}

void	get_sublist(t_list **sublist, char *path, char *opendir_path)
{
	char	*new_opendir_path;
	char	*new_path;
	char	*wc;
	t_list	*file_lst;
	t_list	*new;
	t_list	*new_path_lst;

	new_opendir_path = NULL;
	new_path = NULL;
	wc = NULL;
	file_lst = NULL;
	new = NULL;
	new_path_lst = NULL;
	if (!sublist || !opendir_path || !*opendir_path || !path || !*path)
		ft_exit(EXIT_FAILURE, "Exited in function: get_sublist\nExit due to: argument check fail\n");
	wc = ft_strchr(path, '*');
	if (!wc)
	{
		new = ft_lstnew(path);
		ft_lstadd_back(sublist, new);
	}
	else
	{
		get_file_list(opendir_path, &file_lst);
		if (!file_lst)
			return ;
		get_new_path_list(path, file_lst, &new_path_lst);
		ft_free_list(file_lst);
		if (!new_path_lst)
			return ;
		while(new_path_lst)
		{
			new_path = ft_strdup((char *)new_path_lst->content);
			get_opendir_path(new_path, &new_opendir_path);
			get_sublist(sublist, new_path, new_opendir_path);
			ft_free(new_opendir_path);
			new_path_lst = new_path_lst->next;
		}
		ft_free_list(new_path_lst);
		ft_free(path);
	}
}

void	get_opendir_path(char *path, char **opendir_path_add)
{
	int		i;
	char	*opendir_path;

	i = 0;
	opendir_path = NULL;
	if (!path || !*path || !opendir_path_add)
		ft_exit(EXIT_FAILURE, "Exited in function: get_opendir_path\nExit due to: argument check fail\n");
	while(path[i] && path[i] != '*')
		i++;
	while (i > 0 && path[i] != '/')
		i--;
	if (ft_strncmp(path, "./", 2) && ft_strncmp(path, "../", 3) && ft_strncmp(path, "/", 1))
		opendir_path = ft_strnjoin("./", path, i + 2);
	else
		opendir_path = ft_strndup(path, i);
	*opendir_path_add = opendir_path;
}

void	trim_extra_wc(char	*str, char **path_add)
{
	int		i;
	int		j;
	char	*path;

	i = 0;
	j = 0;
	path = NULL;
	if (!path_add)
		ft_exit(EXIT_FAILURE, "Exited in function: trim_extra_wc\nExit due to: argument check fail\n");
	if (!str)
		return ;
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
	*path_add = path;
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
		ft_exit(EXIT_FAILURE, "Exited in function: expand_wc_node\nExit due to: argument check fail\n");
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
		ft_exit(EXIT_FAILURE, "Exited in function: handle_wildcards\nExit due to: argument check fail\n");
	if (!*av_tab_add || !**av_tab_add)
		return ; 
	av_tab = *av_tab_add;
	ft_tab_to_lst(av_tab, &av_lst);
	node = av_lst;
	while (node)
	{
		if (ft_strchr((char *)node->content, '*'))
			expand_wc_node(node);
		node = node->next;
	}
	ft_free_tab(av_tab);
	av_tab = NULL;
	ft_lst_to_tab(&av_tab, &av_lst);
	*av_tab_add = av_tab;
}
