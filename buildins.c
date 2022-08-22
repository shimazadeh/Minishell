/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/08 16:25:55 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/22 20:06:14 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd(void)
{
	char	*pwd;

	pwd = ft_getcwd();
	if (pwd)
	{
		ft_dprintf(1, "%s\n", pwd);
		ft_free(pwd);
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

int	env(t_list **envp_head)
{
	t_list	*node;

	if (!envp_head || !*envp_head)
		ft_exit(EXIT_FAILURE, "Exited in function: env\nExit due to: argument check fail\n");
	node = *envp_head;
	while (node)
	{
		ft_dprintf(1, "%s\n", (char *)node->content);
		node = node->next;
	}
	return (EXIT_SUCCESS);
}

int	export(char **vars, t_list **envp_head)
{
	t_list	*new;
	t_list	*old;
	char	*tmp;
	int		i;
	int		ret;

	if (!envp_head || !vars)
		ft_exit(EXIT_FAILURE, "Exited in function: export\nExit due to: argument check fail\n");
	if (!vars[0])
	{
		print_sorted_list(envp_head);
		return (EXIT_SUCCESS);
	}
	i = 0;
	ret = EXIT_SUCCESS;
	while (vars[i])
	{
		if (contains_invalid_char(vars[i], "@#$%^&*-+=_<>|\\", 1))
		{
			ft_dprintf(2, "bash: export: `%s': not a valid identifier\n", vars[i]);
			ret = EXIT_FAILURE;
		}
		else
		{
			old = old_var(vars[i], envp_head);
			if (old)
			{
				ft_free(old->content);
				old->content = ft_strdup(vars[i]);
			}
			else
			{
				tmp = ft_strdup(vars[i]);
				new = ft_lstnew(tmp);
				ft_lstadd_back(envp_head, new);
			}
		}
		i++;
	}
	return (ret);
}

int	unset(char **vars, t_list **envp_head)
{
	t_list	*node;
	t_list	*tmp;
	char	*var2;
	int		i;
	int		ret;

	if (!envp_head || !vars)
		ft_exit(EXIT_FAILURE, "Exited in function: unset\nExit due to: argument check fail\n");
	ret = 0;
	if (!vars[0] || !*envp_head)
		return (ret);
	i = 0;
	while (vars[i])
	{
		if (contains_invalid_char(vars[i], "@#$%^&*-+=_<>|\\", 0))
		{
			ft_dprintf(2, "bash: unset: `%s': not a valid identifier\n", vars[i]);
			ret = EXIT_FAILURE;
		}
		else
		{
			var2 = ft_strjoin(vars[i], "=");
			node = *envp_head;
			while (node)
			{
				tmp = node->next;
				if (!ft_strncmp(var2, (char *)node->content, ft_strlen(var2)))
					ft_list_remove_node(envp_head, node);
				node = tmp;
			}
		}
		i++;
	}
	return (ret);
}

int	echo(char **av)
{
	if (!av)
		ft_exit(EXIT_FAILURE, NULL);
	else if (av[0] && !ft_strncmp(av[0], "-n", 3))
		print_tab_nl(&av[1], 0);
	else
		print_tab_nl(av, 1);
	return (EXIT_SUCCESS);
}

int	cd(char *dir, t_list **envp_head)
{
	int		i;
	int		ret1;
	int		ret2;
	char	*cdpath_exp;
	char	*curpath;
	char	*home_exp;
	char	*oldpwd_exp;
	char	*mask;
	char	*prev_compo;
	char	*prev_compo_path;
	char	*pwd_exp;
	char	*tmp;
	char	**cd_paths;
	char	**to_export;

	i = 0;
	ret1 = 0;
	ret2 = 0;
	cdpath_exp = NULL;
	curpath = NULL;
	home_exp = NULL;
	mask = NULL;
	oldpwd_exp = NULL;
	prev_compo = NULL;
	prev_compo_path = NULL;
	pwd_exp = NULL;
	tmp = NULL;
	cd_paths = NULL;
	to_export = NULL;
	if (!envp_head)
		ft_exit(EXIT_FAILURE, "Exited in function: cd\nExit due to: argument check fail\n");
	if (!dir)
	{
		find_env_var("HOME", envp_head, &home_exp);
		if (home_exp)
		{
			dir = ft_strdup(home_exp);
			ft_free(home_exp);
		}
		else
			dir = ft_getcwd();
	}
	else if (!ft_strncmp(dir, "-", 2))
	{
		find_env_var("OLDPWD", envp_head, &oldpwd_exp);
		if (!oldpwd_exp)
		{
			dprintf(2, "bash: cd: OLDPWD not set\n");
			return (EXIT_FAILURE);
		}
		ret1 = cd(oldpwd_exp, envp_head);
		ft_free(oldpwd_exp);
		if (!ret1)
			return (pwd());
		return (ret1);
	}
	else if (dir[0] != '/' && (!ft_strncmp(dir, ".", 2) || !ft_strncmp(dir, "./", 2) || !ft_strncmp(dir, "..", 3) || !ft_strncmp(dir, "../", 3)))
	{
		find_env_var("CDPATH", envp_head, &cdpath_exp);
		if (!cdpath_exp)
		{
			tmp = ft_strjoin("./", dir);
			if (!access(tmp, F_OK))
				curpath = ft_strdup(tmp);
			ft_free(tmp);
		}
		else
		{
			cd_paths = ft_split(cdpath_exp, ':');
			ft_free(cdpath_exp);
			i = 0;
			while (cd_paths[i] && !curpath)
			{
				if (cd_paths[i][ft_strlen(cd_paths[i]) - 1] != '/')
				{
					tmp = cd_paths[i];
					cd_paths[i] = ft_strjoin(tmp, "/");
					ft_free(tmp);
				}
				tmp = ft_strjoin(cd_paths[i], dir);
				if (!access(tmp, F_OK))
					curpath = ft_strdup(tmp);
				ft_free(tmp);
				i++;
			}
			ft_free_tab(cd_paths);
		}
	}
	if (!curpath)
		curpath = ft_strdup(dir);
	ft_free(dir);
	if (curpath[0] != '/')
	{
		pwd_exp = ft_getcwd();
		if ((pwd_exp[ft_strlen(pwd_exp) - 1] != '/'))
		{
			tmp = pwd_exp;
			pwd_exp = ft_strjoin(tmp, "/");
			ft_free(tmp);
		}
		tmp = curpath;
		curpath = (ft_strjoin(pwd_exp, tmp));
		ft_free(tmp);
	}
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
			i++;
			if (curpath[i] == '.')
			{
				prev_compo_2dot_or_root(curpath, mask, i - 2, &prev_compo, &prev_compo_path);
				if (prev_compo && ft_strncmp(prev_compo, "..", 3))
				{
					if (opendir(prev_compo_path) || errno != ENOTDIR)
					{
						mask_prev_compo(mask, curpath, i - 2);
						i++;
					}
					else
						return (EXIT_FAILURE); 
				}
				ft_free(prev_compo);
				ft_free(prev_compo_path);
			}
			while (curpath[i] == '/')
				i++;
		}
		else
		{
			mask[i] = 1;
			i++;
		}
	}
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

int	buildins_dispatch(char **av, t_list **envp_head)
{
	if (!av || !av[0])
		ft_exit(EXIT_FAILURE, "Exited in function: buildins_dispatch\nExit due to: argument check fail\n");
	else if (!ft_strncmp(av[0], "echo", 5))
		return (echo(&av[1]));
	else if (!ft_strncmp(av[0], "cd", 3) && (!av[1] || (av[1] && !av[2])))
		return (cd(av[1], envp_head));
	else if (!ft_strncmp(av[0], "pwd", 4))
		return (pwd());
	else if (!ft_strncmp(av[0], "export", 7))
		return (export(&av[1], envp_head));
	else if (!ft_strncmp(av[0], "unset", 6))
		return (unset(&av[1], envp_head));
	else if (!ft_strncmp(av[0], "env", 4) && !av[1])
		return (env(envp_head));
	else if (!ft_strncmp(av[0], "exit", 5))
		ft_exit(EXIT_SUCCESS, NULL);
	return (127);
}
