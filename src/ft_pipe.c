/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 17:38:31 by shabibol          #+#    #+#             */
/*   Updated: 2022/09/23 16:52:42 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_structure(t_struct **elements, char *str)
{
	t_struct	*copy;
	int			i;
	char		**tab;

	i = -1;
	tab = ft_split_custom(str, '|', 0);
	while (tab[++i])
	{
		copy = ft_alloc(sizeof(t_struct));
		copy->str = ft_strdup(tab[i]);
		copy->cmd = NULL;
		copy->infiles = NULL;
		copy->outfiles = NULL;
		copy->outfile_modes = 0;
		copy->infile_modes = 0;
		copy->tag = 0;
		copy->fds[0] = STDIN_FILENO;
		copy->fds[1] = STDOUT_FILENO;
		copy->wstatus = 0;
		copy->child = 0;
		copy->next = NULL;
		structure_add_back(elements, copy);
		copy = copy->next;
	}
	ft_free_tab(tab);
}

void	assign_str_to_struct(t_struct **elements, char *str)
{
	t_struct	*copy;
	int			i;
	char		**tab;

	i = 0;
	copy = *elements;
	tab = ft_split_custom(str, '|', 0);
	while (tab[i] && copy)
	{
		copy->str = ft_strdup(tab[i]);
		i++;
		copy = copy->next;
	}
	ft_free_tab(tab);
	return ;
}

char	*create_path(char *path)
{
	char	*res;

	if (path[ft_strlen(path) - 1] != '/')
		res = ft_strjoin(path, "/");
	else
		res = ft_strdup(path);
	ft_free(path);
	return (res);
}

char	**extract_env_paths(char *find, t_list **envp_head)
{
	char	**paths;
	char	*temp;
	char	**tab_temp;
	int		k;

	k = 0;
	paths = NULL;
	tab_temp = NULL;
	temp = NULL;
	find_env_var(find, envp_head, &temp);
	if (temp)
	{
		tab_temp = ft_split(temp, ':');
		paths = ft_alloc(sizeof(char *) * (ft_strlen_tab(tab_temp) + 2));
		ft_free(temp);
		k = -1;
		while (tab_temp[++k])
			paths[k] = create_path(tab_temp[k]);
	}
	else
		paths = ft_alloc(sizeof(char *) * 2);
	temp = ft_getcwd();
	paths[k] = create_path(temp);
	paths[++k] = NULL;
	return (ft_free(temp), paths);
}

int	ft_pipe(char *str, t_list **envp_head, int last_exit_code)
{
	char		**parsed_path;
	t_struct	*elements;
	int			exit_code;
	char		**herdoc_files;
	int			here_doc_flag;

	elements = NULL;
	parsed_path = NULL;
	herdoc_files = NULL;
	exit_code = -1;
	here_doc_flag = 0;
	create_structure(&elements, str);
	parsed_path = extract_env_paths("PATH", envp_head);
	str = variable_expansion(str, envp_head, last_exit_code);
	here_doc_flag = number_of_here_doc(str);
	herdoc_files = ft_here_doc(&str, &elements, envp_head, last_exit_code);
	if (!herdoc_files && here_doc_flag > 0)
		exit_code = 2;
	else
	{
		assign_str_to_struct(&elements, str);
		exit_code = execute(&elements, parsed_path, envp_head);
	}
	ft_free_sc(elements);
	return (ft_free_tab(parsed_path), ft_unlink(herdoc_files), exit_code);
}
