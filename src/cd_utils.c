/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 21:38:31 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/25 01:50:40 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prev_compo_2dot_or_root(char *path, char *mask, int i, \
t_cd_curpath_is_dot_vars	v[1])
{
	int		j;
	char	*prev_compo;
	char	*prev_compo_path;

	j = 0;
	prev_compo = NULL;
	prev_compo_path = NULL;
	if (!path || *path != '/' || !mask || !*mask || !v)
		ft_exit(EXIT_FAILURE, "prev_compo_2dot_or_root", "argument check fail");
	while (!mask[i] || path[i] != '/')
		i--;
	if (!i)
		return ;
	j = i;
	while (mask[i - 1] && path[i - 1] != '/')
		i--;
	prev_compo = ft_strndup(&path[i], j - i);
	prev_compo_path = mask_result_str(mask, ft_strndup(path, j));
	v->prev_compo = prev_compo;
	v->prev_compo_path = prev_compo_path;
}

void	mask_prev_compo(char *mask, char *path, int i)
{
	while (!mask[i] || path[i] != '/')
		i--;
	mask[i] = 0;
	i--;
	while (mask[i] && path[i] != '/')
	{
		mask[i] = 0;
		i--;
	}
	mask[i] = 0;
}

char	*mask_result_str(char *mask, char *path)
{
	int		i;
	int		j;
	int		size;
	char	*new_path;

	size = 0;
	i = -1;
	while (path[++i])
		if (mask[i])
			size++;
	new_path = ft_alloc(sizeof(char) * (size + 1));
	new_path[size] = '\0';
	i = 0;
	j = 0;
	while (path[i])
	{
		if (mask[i])
		{
			new_path[j] = path[i];
			j++;
		}
		i++;
	}
	return (new_path);
}
