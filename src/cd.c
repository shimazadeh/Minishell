/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 14:32:29 by aguillar          #+#    #+#             */
/*   Updated: 2022/09/23 17:58:19 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd_init_vars(t_cd_vars v[1])
{
	v->ret = 0;
	v->cd_no_path_ret = NULL;
	v->curpath = NULL;
	v->mask = NULL;
	v->pwd_exp = NULL;
	v->tmp = NULL;
}

int	cd_check(char **dir_tab, char **dir_add)
{
	if (dir_tab[0] && dir_tab[1])
	{
		ft_dprintf(1, "bash: cd: too many arguments\n");
		return (0);
	}
	*dir_add = dir_tab[0];
	return (1);
}

int	cd(char **dir_tab, t_list **envp_head)
{
	t_cd_vars	v[1];
	char		*dir;

	dir = NULL;
	cd_init_vars(v);
	if (!envp_head || !dir_tab)
		ft_exit(EXIT_FAILURE, "cd", "argument check fail");
	if (!cd_check(dir_tab, &dir))
		return (1);
	if (!dir)
		cd_no_arg(&dir, envp_head);
	else if (!ft_strncmp(dir, "-", 2))
		return (cd_hyphen(envp_head));
	else if (cd_no_path_check(dir))
	{
		cd_no_path(&(v->cd_no_path_ret), dir, &(v->curpath), envp_head);
		if (v->cd_no_path_ret && v->cd_no_path_ret[0])
		{
			v->ret = v->cd_no_path_ret[1];
			ft_free(v->cd_no_path_ret);
			return (v->ret);
		}
	}
	cd_path_to_curpath(dir, v);
	return (cd_canon_and_exec(envp_head, v, dir));
}

void	cd_path_to_curpath(char *dir, t_cd_vars *v)
{
	if (!v->curpath)
		v->curpath = ft_strdup(dir);
	v->pwd_exp = ft_getcwd();
	if (v->curpath[0] != '/')
		cd_curpath_no_abs_path(&(v->curpath), v->pwd_exp);
}

int	cd_canon_and_exec(t_list **envp_head, t_cd_vars *v, char *dir)
{
	if (!cd_get_canon_curpath_mask(&(v->mask), v->curpath))
		return (EXIT_FAILURE);
	v->tmp = v->curpath;
	v->curpath = mask_result_str(v->mask, v->tmp);
	ft_free(v->tmp);
	ft_free(v->mask);
	if (chdir(v->curpath) == -1)
	{
		ft_dprintf(1, "bash: cd: %s: ", dir);
		perror(NULL);
		ft_free(v->curpath);
		ft_free(v->pwd_exp);
		return (1);
	}
	ft_free(dir);
	return (cd_to_export_1(v->curpath, v->pwd_exp, envp_head));
}
