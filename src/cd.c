/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 14:32:29 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/24 19:12:57 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd_no_arg(char **dir_add,t_list **envp_head)
{
	char	*dir;
	char	*home_exp;

	dir = NULL;
	home_exp = NULL;
	find_env_var("HOME", envp_head, &home_exp);
	if (home_exp)
	{
		dir = ft_strdup(home_exp);
		ft_free(home_exp);
	}
	else
		dir = ft_getcwd();
	if (dir)
		*dir_add = dir;
}

int	cd_hyphen(t_list **envp_head)
{
	int		ret;
	char	*oldpwd_exp;

	ret = 0;
	oldpwd_exp = NULL;
	find_env_var("OLDPWD", envp_head, &oldpwd_exp);
	if (!oldpwd_exp)
	{
		dprintf(2, "bash: cd: OLDPWD not set\n");
		return (EXIT_FAILURE);
	}
	ret = cd(oldpwd_exp, envp_head);
	ft_free(oldpwd_exp);
	if (!ret)
		return (pwd());
	return (ret);
}

void	cd_curpath_no_abs_path(char **curpath_add, char *pwd_exp)
{
	char	*curpath;
	char	*tmp;

	curpath = NULL;
	tmp = NULL;
	curpath = *curpath_add;
	if ((pwd_exp[ft_strlen(pwd_exp) - 1] != '/'))
	{
		tmp = ft_strdup(pwd_exp);
		pwd_exp = ft_strjoin(tmp, "/");
		ft_free(tmp);
	}
	tmp = curpath;
	curpath = (ft_strjoin(pwd_exp, tmp));
	ft_free(tmp);
	*curpath_add = curpath;
}

int	cd_to_export(char *curpath, char *pwd_exp, t_list **envp_head)
{
	int		ret1;
	int		ret2;
	char	**to_export;

	ret1 = 0;
	ret2 = 0;
	to_export = NULL;
	to_export = ft_alloc(sizeof(char *) * 2);
	to_export[0] = ft_strjoin("PWD=", curpath);
	to_export[1] = NULL;
	ft_free(curpath);
	ret1 = export(to_export, envp_head);
	ft_free(to_export);
	to_export = ft_alloc(sizeof(char *) * 2);
	to_export[0] = ft_strjoin("OLDPWD=", pwd_exp);
	to_export[1] = NULL;
	ft_free(pwd_exp);
	ret2 = export(to_export, envp_head);
	ft_free(to_export);
	if (!ret2)
		return (ret1);
	return (ret2);
}

int	cd(char *dir, t_list **envp_head)
{
	int		ret;
	int		*cd_no_path_ret;
	char	*curpath;
	char	*mask;
	char	*pwd_exp;
	char	*tmp;

	ret = 0;
	cd_no_path_ret = NULL;
	curpath = NULL;
	mask = NULL;
	pwd_exp = NULL;
	tmp = NULL;
	if (!envp_head)
		ft_exit(EXIT_FAILURE, "Exited in function: cd\nExit due to: argument check fail\n");
	if (!dir)
		cd_no_arg(&dir, envp_head);
	else if (!ft_strncmp(dir, "-", 2))
		return (cd_hyphen(envp_head));
	else if (cd_no_path_check(dir))
	{
		cd_no_path(&cd_no_path_ret, dir, &curpath, envp_head);
		if (cd_no_path_ret && cd_no_path_ret[0])
		{
			ret = cd_no_path_ret[1];
			ft_free(cd_no_path_ret);
			return (ret);
		}
	}
	if (!curpath)
		curpath = ft_strdup(dir);
	ft_free(dir);
	pwd_exp = ft_getcwd();
	if (curpath[0] != '/')
		cd_curpath_no_abs_path(&curpath, pwd_exp);
	if (!cd_get_canon_curpath_mask(&mask, curpath))
		return (EXIT_FAILURE);
	tmp = curpath;
	curpath = mask_result_str(mask, tmp);
	ft_free(tmp);
	ft_free(mask);
	if (chdir(curpath) == -1)
	{
		ft_free(curpath);
		ft_free(pwd_exp);
		return (1);
	}
	return (cd_to_export(curpath, pwd_exp, envp_head));
}
