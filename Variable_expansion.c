/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shabibol <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 17:21:24 by shabibol          #+#    #+#             */
/*   Updated: 2022/08/17 17:21:27 by shabibol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*look_up(char *to_find, t_list **envp)
{
	t_list *node;
	char *result;
	int  size;

	size = ft_strlen(to_find);
	node = *envp;
	while(node)
	{
		if (!ft_strncmp(to_find, (char *)node->content, size))
		{
			result = ft_strdup_range((char *)node->content , size, ft_strlen((char *)node->content));
			return (result);
		}
		node = node->next;
	}
	return (NULL);
}

int	variable_expansion(char **str_add, t_list **envp_head, int last_exit_code)
{
	int i;
	int j;
	int	k;
	char *tmp;
	char *tmp2;
	char *find;
	char *str;

	i = 0;
	j = 0;
	k = 0;
	str = *str_add;
	while(str[i])
	{
		if (str[i] && str[i] == '\"')
		{
			k = go_to_closing_char(&str[i]);
			if (k)
			{
				str[i] = ' ';
				i += k;
				str[i] = ' ';
			}
		}
		else if (str[i] && str[i] == '\'')
		{
			k = go_to_closing_char(&str[i]);
			if (k)
			{
				str[i] = ' ';
				i += k;
				str[i] = ' ';
			}
		}
		else if (str[i] && str[i] == '$' && str[i + 1] != ' ')
		{
			i++;
			j = i;
			if (str[i] && str[i] == '?')
				find = ft_itoa(last_exit_code);
			else if (str[i])
			{
				while(str[i] != ' ')
					i++;
				tmp = ft_strdup_range(str, j, i);
				tmp2 = ft_strjoin(tmp, "=");
				find = look_up(tmp2, envp_head);
				ft_free(tmp);
				ft_free(tmp2);
			}
			tmp = ft_strdup_range(str, 0, j);
			tmp2 = ft_strjoin(tmp, find);
			ft_free(tmp);
			tmp = ft_strdup_range(str, i, ft_strlen(str));
			ft_free(str);
			str = ft_strjoin(tmp, tmp2);
			i = 0;
		}
		else
			i++;
	}
	*str_add = str;
	return (0);
}
