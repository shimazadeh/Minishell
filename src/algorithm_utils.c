/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithm_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 19:02:30 by aguillar          #+#    #+#             */
/*   Updated: 2022/09/27 15:23:34 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	go_to_closing_char(char *str)
{
	char	c;
	int		i;

	c = 0;
	i = 0;
	if (!str)
		ft_exit(EXIT_FAILURE, "go_to_closing_char", "argument check fail");
	c = *str;
	i = 1;
	while (str[i] && str[i] != c)
		i++;
	if (str[i])
		return (i);
	return (0);
}

void	rm_outer_par_1(char **str_add)
{
	int		i;
	char	*str;
	char	*tmp;

	i = 0;
	str = NULL;
	tmp = NULL;
	if (!str_add || !*str_add)
		ft_exit(EXIT_FAILURE, "rm_outer_par", "argument check fail");
	str = *str_add;
	rm_outer_par_2(&i, 0, 1, str);
	tmp = str;
	str = ft_strdup(&tmp[i]);
	ft_free(tmp);
	*str_add = str;
}

void	rm_outer_par_2(int *i_add, int j, int flag, char *str)
{
	int	i;

	i = 0;
	while ((str[i] == ' ' || str[i] == '(') && flag)
	{
		if (str[i] == ' ')
			i++;
		else if (str[i] == '(')
		{
			j = i + go_to_closing_par(&str[i], NULL);
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
	*i_add = i;
}

int	go_to_closing_par(char *str, char *par_tab)
{
	int		count;
	int		i;

	if (!str)
		ft_exit(EXIT_FAILURE, "go_to_closing_par", "argument check fail");
	count = 1;
	if (par_tab)
		par_tab[0] = 1;
	i = 0;
	while (str[++i])
	{
		if (str[i] == ')')
			count--;
		if (str[i] == '(')
			count++;
		if (!count && str[i] == ')' && (!par_tab || !par_tab[i]))
		{
			if (par_tab)
				par_tab[i] = 1;
			return (i);
		}
	}
	return (0);
}

int	str_is_only_spaces(char *str)
{
	int	i;

	i = 0;
	if (!str)
		ft_exit(EXIT_FAILURE, "str_is_only_spaces", "argument check fail");
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}
