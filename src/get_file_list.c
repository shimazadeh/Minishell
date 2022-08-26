/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_file_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 00:20:51 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/26 22:38:10 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	read_dir_content(struct dirent **dir_content_add, DIR *stream)
{
	struct dirent	*dir_content;

	if (!dir_content_add)
		ft_exit(EXIT_FAILURE, "read_dir_content", "argument check fail");
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
		ft_exit(EXIT_FAILURE, "get_file_tab", "argument check fail");
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
		return ;
	*file_lst_add = file_lst;
}
