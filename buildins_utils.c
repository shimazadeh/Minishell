/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 20:50:57 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/21 01:33:21 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*old_var(char *var, t_list **envp_head)
{
	t_list	*old;
	int		i;

	if (!var || !envp_head)
		ft_exit(EXIT_FAILURE, NULL);
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
		ft_exit(EXIT_FAILURE, NULL);
	if (!*envp_head)
		return ;
	node = *envp_head;
	size = ft_lstsize(node);
	tab = ft_alloc(sizeof(char) * (size + 1));
	if (!tab)
		ft_exit(EXIT_FAILURE, NULL);
	ft_bzero(tab, sizeof(char) * (size + 1));
	while (print_lowest_ascii(node, tab));

}

int	print_lowest_ascii(t_list *node, char *tab)
{
	int		i;
	char	*lowest_ascii;
	int		lowest_ascii_mask;

	if (!node || !tab)
		ft_exit(EXIT_FAILURE, NULL);
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
		//	dprintf(2, "lowestascii %s\n", lowest_ascii);
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
		ft_exit(EXIT_FAILURE, NULL);
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

void	find_env_var(char *var_name, t_list **envp_head, char **var_exp)
{
	t_list	*node;

	if (!var_name || !envp_head)
		ft_exit(EXIT_FAILURE, NULL);
	node = *envp_head;
	var_name = ft_strjoin(var_name, "=");
	if (!var_name)
		ft_exit(errno, NULL);
	while (node)
	{
		if (!ft_strncmp(var_name, (char *)node->content, ft_strlen(var_name)))
		{
			*var_exp = ft_strchr((char *)node->content, '=');
			++(*var_exp);
				return ;
		}
		node = node->next;
	}
	*var_exp = NULL;
}

void	prev_compo_2dot_or_root(char *path, int i, char **prev_compo_add, char **prev_compo_path_add)
{
	char	*path_dup;
	char	*prev_compo;
	char	*prev_compo_path;

	path_dup = NULL;
	prev_compo = NULL;
	prev_compo_path = NULL;
	if (!path || !prev_compo_add || !prev_compo_path_add)
		ft_exit(EXIT_FAILURE, NULL);
	if (i < 0)
		return ;
	path_dup = ft_strdup(path);
	if (!path_dup)
		ft_exit(errno, NULL);
	while (i >= 0 && path_dup[i] == '/')
		i--;
	if (i == -1)
	{
		ft_free(path_dup);
		return ;
	}
	// finish this function
	// code the hyphen / CD_PATH cd thing
	// finish wc hanling
	// norm this bitch
	// deal with infunction checks and error_msg;
}

void	mask_prev_compo(char *mask, char *curpath, int i)
{
	if (i < 0)
		return ;
	while (i >= 0 && curpath[i] == '/')
	{
		mask[i] = 0;
		i--;
	}
	while (i >= 0 && curpath[i] != '/')
	{
		mask[i] = 0;
		i--;
	}
}

char	*mask_result_str(char *mask, char *curpath)
{
	int		i;
	int		j;
	int		size;
	char	*str;

	size = 0;
	i = 0;
	while (curpath[i])
	{
		if (mask[i])
			size++;
		i++;
	}
	str = ft_alloc(sizeof(char) * (size + 1));
	if (!str)
		ft_exit(errno, NULL);
	str[size] = 0;
	i = 0;
	j = 0;
	while (curpath[i])
	{
		if (mask[i])
		{
			str[j] = curpath[i];
			j++;
		}
		i++;
	}
	return (str);
}
