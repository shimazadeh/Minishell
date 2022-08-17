/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 20:50:57 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/12 04:25:22 by aguillar         ###   ########.fr       */
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
	tab = ft_alloc(sizeof(char) * size);
	if (!tab)
		ft_exit(EXIT_FAILURE, NULL);
	ft_bzero(tab, sizeof(char) * size);
	while (print_lowest_ascii(node, tab));

}

int	print_lowest_ascii(t_list *node, char *tab)
{
	int		i;
	char	*lowest_ascii;
	int		ret;

	if (!node || !tab)
		ft_exit(EXIT_FAILURE, NULL);
	lowest_ascii = (char *)node->content;
	ret = 0;
	i = 0;
	while (node)
	{
		if (!tab[i] && ft_strncmp(lowest_ascii, (char *)node->content, -1) < 0)
		{
			ret = 1;
			lowest_ascii = node->content;
		}
		i++;
		node = node->next;
	}
	if (ret)
		ft_dprintf(1, "%s\n", (char *)node->content);
	return (ret);
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
		if (ft_strchr(str, id[i]) && !(j == 1 && i && str[i] == '='))
			return (0);
	}
	return (1);
}


void	print_tab_nl(char **tab, int nl)
{
	int	i;

	i = 0;
	if (nl)
	{
		while (tab[i])
		{
			ft_dprintf(1, "%s\n", tab[i]);
			i++;
		}
		return ;
	}
	while (tab[i])
	{
		ft_dprintf(1, "%s", tab[i]);
		i++;
	}
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
			*var_exp = ft_strchr(node->content, '=');
			*var_exp = *var_exp + 1;
				return ;
		}
		node = node->next;
	}
	*var_exp = NULL;
}


void	mask_prev_compo(char *mask, char *curpath, int i)
{
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
	while (mask[i])
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
