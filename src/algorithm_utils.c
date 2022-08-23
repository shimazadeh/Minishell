/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithm_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 19:02:30 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/24 00:35:51 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	str_is_only_spaces(char *str)
{
	int	i;

	i = 0;
	if (!str)
		ft_exit(EXIT_FAILURE, "Exited in function: str_is_only_spaces\nExit due to: argument check fail\n");
	while (str[i])
	{
		if (str[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}

int	go_to_closing_par(char *str)
{
	int		count;
	int		i;

	count = 0;
	i = 0;
	if (!str)
		ft_exit(EXIT_FAILURE, "Exited in function: go_to_closing_char\nExit due to: argument check fail\n");
	count = 1;
	i = 1;
	while (str[i])
	{
		if (str[i] == ')')
			count--;
		if (str[i] == '(')
			count++;
		if (!count && str[i] == ')')
			return (i);
		i++;
	}
	return (0);
}

void	rm_outer_par(char **str_add)
{
	int		flag;
	int		i;
	int		j;
	char	*str;
	char	*tmp;

	flag = 0;
	i = 0;
	j = 0;
	str = NULL;
	tmp = NULL;
	if (!str_add || !*str_add)
		ft_exit(EXIT_FAILURE, "Exited in function: rm_outer_par\nExit due to: argument check fail\n");
	flag = 1;
	str = *str_add;
	while ((str[i] == ' ' || str[i] == '(') && flag)
	{
		if (str[i] == ' ')
			i++;
		if (str[i] == '(')
		{
			j = go_to_closing_par(&str[i]);
			if (j && str_is_only_spaces(&str[j + 1]))
			{
				i++;
				while (j > 0 && str[j - 1] == ' ')
					j--;
				str[j] = '\0';
			}
			else
				flag = 0;
		}
	}
	tmp = ft_strdup(&str[i]);
	*str_add = tmp;
	ft_free(str);
}

int	go_to_closing_char(char *str)
{
	char	c;
	int		i;

	c = 0;
	i = 0;
	if (!str)
		ft_exit(EXIT_FAILURE, NULL);
	c = *str;
	i = 1;
	while (str[i] && str[i] != c)
		i++;
	if (str[i])
		return (i);
	return (0);
}

void	tab_to_list(char **tab, t_list **lst_head)
{
	int		i;
	t_list	*new;
	char	*str;

	i = 0;
	if (!lst_head)
		ft_exit(EXIT_FAILURE, "Exited in function: tab_to_list\nExit due to: argument check fail\n");
	if (!tab || !tab[0])
		return ;
	while (tab[i])
	{
		str = ft_strdup(tab[i]);
		new = ft_lstnew(str);
		ft_lstadd_back(lst_head, new);
		i++;
	}
}

int	even_par_nbr(char *str)
{
	int	i;
	int	par;

	i = 0;
	par = 0;
	while (str[i])
	{
		if (str[i] == '(')
			par++;
		if (str[i] == ')')
			par--;
		i++;
	}
	if (par)
		return (0);
	return (1);
}
