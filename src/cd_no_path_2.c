/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_no_path_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 19:30:58 by aguillar          #+#    #+#             */
/*   Updated: 2022/09/20 15:10:35 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd_no_path_cdpath_exp_init_vars(t_cd_no_path_cdpath_exp_vars v[1], \
char *cdpath_exp)
{
	v->curpath = NULL;
	v->tmp = NULL;
	v->cd_paths = NULL;
	v->cd_no_path_ret = NULL;
	v->cd_paths = ft_split(cdpath_exp, ':');
	ft_free(cdpath_exp);
	v->cd_no_path_ret = ft_alloc(sizeof(int) * 2);
	ft_bzero(v->cd_no_path_ret, sizeof(int) * 2);
}

void	cd_no_path_cdpath_exp(int **cd_no_path_ret_add, \
char *cdpath_exp, char *dir, t_list **envp_head)
{
	int								i;
	t_cd_no_path_cdpath_exp_vars	v[1];

	i = 0;
	cd_no_path_cdpath_exp_init_vars(v, cdpath_exp);
	while (v->cd_paths[i])
	{
		if (v->cd_paths[i][ft_strlen(v->cd_paths[i]) - 1] != '/')
		{
			v->tmp = v->cd_paths[i];
			v->cd_paths[i] = ft_strjoin(v->tmp, "/");
			ft_free(v->tmp);
		}
		v->tmp = ft_strjoin(v->cd_paths[i], dir);
		if (!access(v->tmp, F_OK))
		{
			handle_valid_cdpath(v, envp_head);
			*cd_no_path_ret_add = v->cd_no_path_ret;
			return ;
		}
		ft_free(v->tmp);
		i++;
	}
	ft_free_tab(v->cd_paths);
	*cd_no_path_ret_add = v->cd_no_path_ret;
}

void	handle_valid_cdpath(t_cd_no_path_cdpath_exp_vars v[1], \
t_list **envp_head)
{
	v->curpath = ft_strdup(v->tmp);
	ft_free(v->tmp);
	ft_free_tab(v->cd_paths);
	v->cd_no_path_ret[0] = 1;
	v->cd_no_path_ret[1] = cd(&(v->curpath), envp_head);
	if (!v->cd_no_path_ret[1])
		v->cd_no_path_ret[1] = pwd();
}
