/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_no_path_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 19:07:02 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/24 19:33:13 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd_no_path_check(char *dir)
{
	if (dir[0] != '/' && ft_strncmp(dir, ".", 2)
		&& ft_strncmp(dir, "./", 2)
		&& ft_strncmp(dir, "..", 3)
		&& ft_strncmp(dir, "../", 3))
		return (1);
	return (0);
}

void	cd_no_path(int	**cd_no_path_ret_add, char *dir, \
char **curpath_add, t_list **envp_head)
{
	char	*cdpath_exp;
	char	*curpath;
	int		*cd_no_path_ret;

	cdpath_exp = NULL;
	curpath = NULL;
	cd_no_path_ret = NULL;
	find_env_var("CDPATH", envp_head, &cdpath_exp);
	if (!cdpath_exp)
		cd_no_path_no_cdpath_exp(dir, &curpath);
	else
	{
		cd_no_path_cdpath_exp(&cd_no_path_ret, cdpath_exp, dir, envp_head);
		*cd_no_path_ret_add = cd_no_path_ret;
	}
	if (curpath)
		*curpath_add = curpath;
}

void	cd_no_path_no_cdpath_exp(char *dir, char **curpath_add)
{
	char	*curpath;
	char	*tmp;

	curpath = NULL;
	tmp = NULL;
	tmp = ft_strjoin("./", dir);
	if (!access(tmp, F_OK))
		curpath = ft_strdup(tmp);
	ft_free(tmp);
	if (curpath)
		*curpath_add = curpath;
}
