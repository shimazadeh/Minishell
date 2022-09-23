/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_get_canon_curpath_mask.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 19:12:22 by aguillar          #+#    #+#             */
/*   Updated: 2022/09/23 14:06:28 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd_get_canon_curpath_mask(char **mask_add, char *curpath)
{
	int		i;
	char	*mask;

	i = 0;
	mask = NULL;
	cd_init_mask(&mask, curpath, &i);
	while (curpath[i])
	{
		if (curpath[i] == '/')
		{
			mask[i] = 1;
			while (curpath[i] == '/')
				i++;
		}
		else if (curpath[i] == '.')
		{
			if (!cd_curpath_is_dot(curpath, mask, &i))
				return (0);
		}
		else
			mask[i++] = 1;
	}
	*mask_add = mask;
	return (1);
}

void	cd_init_mask(char **mask_add, char *curpath, int *i_add)
{
	int		i;
	char	*mask;

	mask = ft_alloc(sizeof(char) * (ft_strlen(curpath) + 1));
	ft_bzero(mask, (sizeof(char) * (ft_strlen(curpath) + 1)));
	mask[0] = 1;
	i = 1;
	if (!ft_strncmp(&curpath[1], "//", 2))
	{
		while (curpath[i] == '/')
			i++;
	}
	else if (curpath[1] == '/')
	{
		mask[1] = 1;
		i++;
	}
	*i_add = i;
	*mask_add = mask;
}

void	cd_curpath_is_dot_init_vars(t_cd_curpath_is_dot_vars v[1], \
int *j_add, int *i_add)
{
	int	i;

	i = 0;
	v->prev_compo = NULL;
	v->prev_compo_path = NULL;
	i = *j_add;
	i++;
	*i_add = i;
}

int	cd_curpath_is_dot(char *curpath, char *mask, int *j_add)
{
	int							i;
	t_cd_curpath_is_dot_vars	v[1];
	DIR							*op;

	i = 0;
	cd_curpath_is_dot_init_vars(v, j_add, &i);
	if (curpath[i] == '.')
	{
		prev_compo_2dot_or_root(curpath, mask, i - 2, v);
		if (v->prev_compo && ft_strncmp(v->prev_compo, "..", 3))
		{
			op = opendir(v->prev_compo_path);
			if (op || errno != ENOTDIR)
				mask_prev_compo(mask, curpath, -2 + i++);
			else
				return (free(op), 0);
			closedir(op);
		}
		ft_free(v->prev_compo);
		ft_free(v->prev_compo_path);
	}
	while (curpath[i] == '/')
		i++;
	*j_add = i;
	return (1);
}
