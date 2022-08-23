/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_submod_and_sep.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 00:37:38 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/24 00:56:47 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	split_submod_and_sep(char **str_add, t_list **submod_head,
t_list **sep_head)
{
	int		count;
	int		i;
	int		j;
	char	*sep;
	char	*str;
	char	*submod;
	t_list	*tmp_node;

	count = 0;
	i = 0;
	j = 0;
	sep = NULL;
	str = NULL;
	submod = NULL;
	tmp_node = NULL;
	if (!str_add || !*str_add || !submod_head || !sep_head)
		ft_exit(EXIT_FAILURE, "Exited in function: split_submod_and_sep\nExit due to: argument check fail\n");
	rm_outer_par(str_add);
	str = *str_add;
	while (str[i])
	{
		if (str[i] == '\"' || str[i] == '\'')
			i += go_to_closing_char(&str[i]);
		else if (str[i] == '(')
			count++;
		else if (str[i] == ')')
			count--;
		if (count == 0 && (ft_strncmp(&str[i], " || ", 4) == 0 || ft_strncmp(&str[i], " && ", 4) == 0))
		{
			submod = ft_strndup(&str[j], i - j);
			if (ft_strncmp(&str[i], " || ", 4) == 0)
				sep = ft_strdup("2");
			else if (ft_strncmp(&str[i], " && ", 4) == 0)
				sep = ft_strdup("1");
			tmp_node = ft_lstnew(submod);
			ft_lstadd_back(submod_head, tmp_node);
			tmp_node = ft_lstnew(sep);
			ft_lstadd_back(sep_head, tmp_node);
			i += 3;
			j = i + 1;
		}
		else if (count == 0 && !str[i + 1] && submod)
		{
			submod = ft_strndup(&str[j], i + 1 - j);
			tmp_node =  ft_lstnew(submod);
			ft_lstadd_back(submod_head, tmp_node);
		}
		i++;
	}
}