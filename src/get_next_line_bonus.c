/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 14:13:12 by aguillar          #+#    #+#             */
/*   Updated: 2022/09/08 19:12:27 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_join_contents(t_list *first)
{
	t_list	*current;
	char	*str;
	char	*tmp;

	current = first;
	str = 0;
	while (current)
	{
		tmp = ft_strjoin(str, current->content);
		ft_free(str);
		if (!tmp)
		{
			ft_free_list(first);
			return (NULL);
		}
		str = tmp;
		current = current->next;
	}
	ft_free_list(first);
	if (!str[0])
	{
		ft_free(str);
		return (0);
	}
	return (str);
}

t_list	*ft_expand_list(t_list *first, t_list *last, char *str, int size)
{
	t_list	*new;

	new = ft_alloc(sizeof(t_list));
	if (!new)
	{
		ft_free_list(first);
		return (NULL);
	}
	if (!size)
	{
		new->content = ft_strjoin(0, str);
		if (!new->content)
			return (ft_free_list(first), NULL);
	}
	else
	{
		new->content = ft_alloc(sizeof(char) * (size + 1));
		if (!new->content)
			return (ft_free_list(first), NULL);
	}
	if (last)
		last->next = new;
	new->next = 0;
	return (new);
}

char	*get_next_line_2(int fd, char **tab, t_list *first, t_list *last)
{
	int		i;
	char	*j;

	i = 1;
	j = ft_strchr(last->content, 10);
	while (i && !j)
	{
		last = ft_expand_list(first, last, 0, (int)BUFFER_SIZE);
		if (!last)
			return (0);
		i = read(fd, last->content, (int)BUFFER_SIZE);
		((char *)(last->content))[i] = 0;
		j = ft_strchr(last->content, 10);
	}
	if (j && *(j + 1))
	{
		tab[fd] = ft_strjoin(0, (j + 1));
		if (!tab[fd])
		{
			ft_free_list(first);
			return (NULL);
		}
		*(j + 1) = 0;
	}
	return (ft_join_contents(first));
}

char	*get_next_line(int fd)
{
	static char	*tab[1024];
	t_list		*first;
	t_list		*last;
	char		*str;

	str = 0;
	if (read(fd, str, 0) < 0 || fd < 0 || fd > 1024 || (int)BUFFER_SIZE < 1)
		return (0);
	first = 0;
	last = 0;
	last = ft_expand_list(first, last, tab[fd], 0);
	ft_free(tab[fd]);
	tab[fd] = 0;
	if (!last)
		return (0);
	first = last;
	return (get_next_line_2(fd, tab, first, last));
}
