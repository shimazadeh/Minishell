/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_wildcards_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 00:20:00 by aguillar          #+#    #+#             */
/*   Updated: 2022/09/27 15:47:12 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	trim_extra_wc(char	*str, char **path_add)
{
	int		i;
	char	*path;

	i = 0;
	path = NULL;
	if (!path_add)
		ft_exit(EXIT_FAILURE, "trim_extra_wc", "argument check fail");
	if (!str)
		return ;
	get_str_size(0, 0, &i, str);
	path = ft_alloc(sizeof(char) * (i + 1));
	fill_str(0, 0, str, path);
	*path_add = path;
}

void	get_str_size(int i, int j, int *j_add, char *str)
{
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
	*j_add = j;
}

void	fill_str(int i, int j, char *str, char *path)
{
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
}

void	get_opendir_path(char *path, char **opendir_path_add)
{
	int		i;
	char	*opendir_path;

	i = 0;
	opendir_path = NULL;
	if (!path || !*path || !opendir_path_add)
		ft_exit(EXIT_FAILURE, "get_opendir_path", "argument check fail");
	while (path[i] && path[i] != '*')
		i++;
	while (i > 0 && path[i] != '/')
		i--;
	if (i == 0 && path[i] == '/')
		i = 1;
	if (ft_strncmp(path, "./", 2) && ft_strncmp(path, "../", 3) \
		&& ft_strncmp(path, "/", 1))
		opendir_path = ft_strnjoin("./", path, i + 2);
	else
		opendir_path = ft_strndup(path, i);
	*opendir_path_add = opendir_path;
}
