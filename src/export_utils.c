/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 14:16:52 by aguillar          #+#    #+#             */
/*   Updated: 2022/09/23 18:14:06 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_sorted_list(t_list **envp_head)
{
	int		i;
	int		size;
	char	*tab;
	t_list	*node;

	i = 0;
	size = 0;
	tab = NULL;
	node = NULL;
	if (!envp_head)
		ft_exit(EXIT_FAILURE, "print_sorted_list", "argument check fail");
	if (!*envp_head)
		return ;
	i = 1;
	node = *envp_head;
	size = ft_lstsize(node);
	tab = ft_alloc(sizeof(char) * (size + 1));
	ft_bzero(tab, sizeof(char) * (size + 1));
	while (i)
		i = print_lowest_ascii(-1, 0, node, tab);
}

int	print_lowest_ascii(int i, int lowest_ascii_mask, t_list *node, char *tab)
{
	char	*lowest_ascii;

	lowest_ascii = NULL;
	if (!node || !tab)
		ft_exit(EXIT_FAILURE, "print_lowest_ascii", "argument check fail");
	i = -1;
	while (tab[++i] == '1')
		node = node->next;
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
	print_export_var(lowest_ascii);
	return (1);
}

void	print_export_var(char *lowest_ascii)
{
	char	*eq_add;
	int		i;
	char	*tmp;

	i = 0;
	if (ft_strncmp(lowest_ascii, "_=", 2))
	{
		eq_add = ft_strchr(lowest_ascii, '=');
		if (eq_add)
		{
			while (lowest_ascii[i] && lowest_ascii[i] != '=')
				i++;
			tmp = ft_strndup(lowest_ascii, i + 1);
			ft_dprintf(1, "export %s", tmp);
			ft_free(tmp);
			ft_dprintf(1, "\"%s\"\n", ++eq_add);
		}
		else
			ft_dprintf(1, "export %s\n", lowest_ascii);
	}
}

t_list	*old_var(char *var, t_list **envp_head)
{
	t_list	*old;
	int		i;

	if (!var || !envp_head)
		ft_exit(EXIT_FAILURE, "old_var", "argument check fail");
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
