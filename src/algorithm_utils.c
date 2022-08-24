/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithm_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 19:02:30 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/24 13:01:04 by aguillar         ###   ########.fr       */
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
		ft_exit(EXIT_FAILURE, NULL);
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
		ft_exit(EXIT_FAILURE, "Exited in function: rm_outer_par\nExit due to: argument check fail\n");
	str = *str_add;
	rm_outer_par_2(&i, 0, 1, str);
	tmp = ft_strdup(&str[i]);
	*str_add = tmp;
	ft_free(str);
}

void	rm_outer_par_2(int *i_add, int j, int flag, char *str)
{
	int	i;

	i = 0;
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
	*i_add = i;
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
