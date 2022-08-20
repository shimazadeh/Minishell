/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/08 16:25:55 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/20 04:30:11 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
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
		ft_exit(EXIT_FAILURE, NULL);
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
		ft_exit(EXIT_FAILURE, NULL);
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
				if (!old->content)
					ft_exit(EXIT_FAILURE, NULL);
			}
			else
			{
				tmp = ft_strdup(vars[i]);
				if (!tmp)
					ft_exit(EXIT_FAILURE, NULL);
				new = ft_lstnew(tmp);
				if (!new)
					ft_exit(EXIT_FAILURE, NULL);
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
		ft_exit(EXIT_FAILURE, NULL);
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
			if (!var2)
				ft_exit(EXIT_FAILURE, NULL);
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
	char	*pwd;
	char	*var_exp;
	char	*curpath;
	char	**cd_paths;
	char	*tmp;
	int		i;
	char	*mask;
	char	**to_export;
	int		ret1;
	int		ret2;

	curpath = NULL;
	if (!dir)
	{
		find_env_var("HOME", envp_head, &var_exp);
		if (var_exp && *var_exp)
		{
			dir = ft_strdup(var_exp);
			if (!dir)
				ft_exit(errno, NULL);
		}
		else
		{
			dir = getcwd(NULL, 0);
			if (!dir)
				ft_exit(errno, NULL);
		}
	}
	else if (dir[0] != '/' && dir[0] != '.' && ft_strncmp(dir, "..", 2))
	{
		find_env_var("CDPATH", envp_head, &var_exp);
		if (!var_exp || !*var_exp)
		{
			tmp = ft_strjoin("./", dir);
			if (!tmp)
				ft_exit(errno, NULL);
			if (!access(tmp, F_OK))
			{
				curpath = ft_strdup(tmp);
				if (!curpath)
					ft_exit(errno, NULL);
			}
			ft_free(tmp);
		}
		else
		{
			cd_paths = ft_split(var_exp, ':');
			if (!cd_paths)
				ft_exit(errno, NULL);
			i = 0;
			while (cd_paths[i] && !curpath)
			{
				if (cd_paths[i][ft_strlen(cd_paths[i]) - 1] != '/')
				{
					tmp = cd_paths[i];
					cd_paths[i] = ft_strjoin(tmp, "/");
					if (!cd_paths[i])
						ft_exit(errno, NULL);
					ft_free(tmp);
				}
				tmp = ft_strjoin(cd_paths[i], dir);
				if (!access(tmp, F_OK))
				{
					curpath = ft_strdup(tmp);
					if (!curpath)
						ft_exit(errno, NULL);
				}
				ft_free(tmp);
				i++;
			}
			ft_free_tab(cd_paths);
		}
	}
	if (!curpath)
		curpath = dir;
	if (curpath[0] != '/')
	{
		pwd = getcwd(NULL, 0);
		if (!pwd)
			ft_exit(errno, NULL);
		if (pwd[ft_strlen(pwd) - 1] != '/')
		{
			tmp = pwd;
			pwd = ft_strjoin(tmp, "/");
			ft_free(tmp);
		}
		tmp = curpath;
		curpath = (ft_strjoin(pwd, tmp));
		ft_free(tmp);
	}
	tmp = curpath;
	i = 0;
	mask = ft_alloc(sizeof(char) * (ft_strlen(curpath) + 1));
	ft_bzero(mask, (sizeof(char) * (ft_strlen(curpath) + 1)));
	while (curpath[i])
	{
		if (i == 0 && !ft_strncmp(curpath, "///", 3))
		{
			mask[0] = 1;
			while (curpath[i] == '/')
				i++;
		}
		else if (i != 0 && !ft_strncmp(&curpath[i], "//", 2))
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
				mask_prev_compo(mask, curpath, i - 2);
				i++;
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
	if (chdir(curpath) == -1)
		return (1);
	to_export = ft_alloc(sizeof(char *) * 2);
	to_export[0] = ft_strjoin("PWD=", curpath);
	if (!to_export)
		ft_exit(errno, NULL);
	to_export[1] = NULL;
	ft_free(curpath);
	ret1 = export(to_export, envp_head);
	ft_free(to_export);
	to_export = ft_alloc(sizeof(char *) * 2);
	to_export[0] = ft_strjoin("OLDPWD=", pwd);
	if (!to_export)
		ft_exit(errno, NULL);
	to_export[1] = NULL;
	ft_free(pwd);
	ret2 = export(to_export, envp_head);
	ft_free(to_export);
	if (!ret2)
		return (ret1);
	return (ret2);
}

int	buildins_dispatch(char **av, t_list **envp_head)
{
	if (!av || !av[0])
		ft_exit(EXIT_FAILURE, NULL);
	if (!ft_strncmp(av[0], "echo", 5))
		return (echo(&av[1]));
	if (!ft_strncmp(av[0], "cd", 3) && (!av[1] || (av[1] && !av[2])))
		return (cd(av[1], envp_head));
	if (!ft_strncmp(av[0], "pwd", 4))
		return (pwd());
	if (!ft_strncmp(av[0], "export", 7))
		return (export(&av[1], envp_head));
	if (!ft_strncmp(av[0], "unset", 6))
		return (unset(&av[1], envp_head));
	if (!ft_strncmp(av[0], "env", 4) && !av[1])
		return (env(envp_head));
	if (!ft_strncmp(av[0], "exit", 5))
		ft_exit(EXIT_SUCCESS, NULL);
	return (127);
}
