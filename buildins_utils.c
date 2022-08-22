/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 20:50:57 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/22 18:52:06 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*old_var(char *var, t_list **envp_head)
{
	t_list	*old;
	int		i;

	if (!var || !envp_head)
		ft_exit(EXIT_FAILURE, "Exited in function: old_var\nExit due to: argument check fail\n");
	i = 0;
	while (var[i] && var[i] != '=')
		i++;
	old = *envp_head;
	while (old)
	{
		if (!strncmp((char *)old->content, var, i))
			return (old);
		old = old->next;
	}
	return (NULL);
}

void	print_sorted_list(t_list **envp_head)
{
	char	*tab;
	int		size;
	t_list	*node;

	if (!envp_head)
		ft_exit(EXIT_FAILURE, "Exited in function: print_sorted_list\nExit due to: argument check fail\n");
	if (!*envp_head)
		return ;
	node = *envp_head;
	size = ft_lstsize(node);
	tab = ft_alloc(sizeof(char) * (size + 1));
	ft_bzero(tab, sizeof(char) * (size + 1));
	while (print_lowest_ascii(node, tab));

}

int	print_lowest_ascii(t_list *node, char *tab)
{
	int		i;
	char	*lowest_ascii;
	int		lowest_ascii_mask;

	if (!node || !tab)
		ft_exit(EXIT_FAILURE, "Exited in function: print_lowest_ascii\nExit due to: argument check fail\n");
	i = 0;
	while (tab[i] == '1')
	{
		i++;
		node = node->next;
	}
	if (!node)
		return (0);
	lowest_ascii = (char *)node->content;
	lowest_ascii_mask = i;
	while (node)
	{
		if (!tab[i] && ft_strncmp(lowest_ascii, (char *)node->content, -1) >= 0)
		{
			lowest_ascii = (char *)node->content;
			lowest_ascii_mask = i;
		}
		i++;
		node = node->next;
	}
	tab[lowest_ascii_mask] = '1';
	ft_dprintf(1, "%s\n", lowest_ascii);
	return (1);
}

int	ft_list_remove_node(t_list **lst_head, t_list *node)
{
	t_list	new_first[1];
	t_list	*prev;
	t_list	*current;

	new_first->next = *lst_head;
	new_first->content = NULL;
	prev = new_first;
	current = *lst_head;
	while (current)
	{
		if (current == node)
		{
			if (current == *lst_head)
				*lst_head = current;
			prev->next = current->next;
			ft_free(current->content);
			ft_free(current);
			return (1);
		}
		current = current->next;
		prev = prev->next;
	}
	return (0);
}

int	contains_invalid_char(char *str, char *id, int j)
{
	int	i;

	i = 0;
	while (id[i])
	{
		if (ft_strchr(str, id[i]) && !(j == 1 && i && ft_strchr(str, '=')))
			return (1);
		i++;
	}
	return (0);
}


void	print_tab_nl(char **tab, int nl)
{
	int	i;

	i = 0;
	if (!tab)
		ft_exit(EXIT_FAILURE, "Exited in function: print_tab_nl\nExit due to: argument check fail\n");
	while (tab[i])
	{
		ft_dprintf(1, "%s", tab[i]);
		if (tab[i + 1])
			ft_dprintf(1, " ", tab[i]);
		i++;
	}
	if (nl)
		ft_dprintf(1, "\n", tab[i]);
}

void	find_env_var(char *var_name, t_list **envp_head, char **var_exp_add)
{
	int		size;
	char	*var_exp;
	char	*var_name_eq;
	t_list	*node;

	size = 0;
	var_exp = NULL;
	var_name_eq = NULL;
	node = NULL;
	if (!var_name || !envp_head || !var_exp_add)
		ft_exit(EXIT_FAILURE, "Exited in function: find_env_var\nExit due to: argument check fail\n");
	node = *envp_head;
	var_name_eq = ft_strjoin(var_name, "=");
	size = ft_strlen(var_name_eq);
	while (node)
	{
		if (!ft_strncmp(var_name_eq, (char *)node->content, size) && (char)(((char *)(node->content))[size]))
			var_exp = ft_strdup((char *)(&((char *)(node->content))[size]));
		node = node->next;
	}
	*var_exp_add = var_exp;
}

void	prev_compo_2dot_or_root(char *path, char *mask, int i, char **prev_compo_add, char **prev_compo_path_add)
{
	int		j;
	char	*prev_compo;
	char	*prev_compo_path;
	char	*tmp;

	j = 0;
	prev_compo = NULL;
	prev_compo_path = NULL;
	tmp = NULL;
	if (!path || *path != '/' || !mask || !*mask || !prev_compo_add || !prev_compo_path_add)
		ft_exit(EXIT_FAILURE, "Exited in function: prev_compo_2dot_or_root\nExit due to: argument check fail\n");
	while (!mask[i] || path[i] != '/')
		i--;
	if (!i)
		return ;
	j = i;
	while (mask[i - 1] && path[i - 1] != '/')
		i--;
	prev_compo = ft_strndup(&path[i], j - i);
	tmp = ft_strndup(path, j);
	prev_compo_path = mask_result_str(mask, tmp);
	ft_free(tmp);
	*prev_compo_add = prev_compo;
	*prev_compo_path_add = prev_compo_path;
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
	i = 0;
	while (path[i])
	{
		if (mask[i])
			size++;
		i++;
	}
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

char	*ft_getcwd(void)
{
	char	*pwd;
	char	*tmp;

	pwd = NULL;
	tmp = getcwd(NULL, 0);
	if (!tmp)
		ft_exit(errno, "Exited in function: ft_getpwd\nExit due to: getcwd fail\n");
	pwd = ft_strdup(tmp);
	free(tmp);
	return (pwd);
}