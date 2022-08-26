/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 01:59:39 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/26 18:24:43 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
#include "minishell.h"

void	tilde_expansion_handling(char **str_add)
{
	int		i;
	char	*str;

	i = 0;
	str = NULL;
	str = *str_add;
	while (str[i])
	{
		if (str[i] == '~' && valid_tilde_conditions(i, str))
			i+= expand_current_tilde(i, &str);
		else
			i++;
	}
	*str_add = str;
}

int	valid_tilde_condition(int i, char *str)
{
	if (i == 0)
		return (1);
	if (str[--i] == ' ')
		return (1);
	if (i == 0)
		return (0);
	if (str[i] == '=')
	{
		while (--i > 0 && str[i] != ' ')
		{
			if (str[i] == ' ')
				return (0);
		}
		return (1);
	}
	else if (str[i] == ':')
	{
		while (--i > 0 && str[i] != ' ')
		{
			if (str[i] == ' ')
				return (1);
		}
	}
	return (0);
}

int	expand_current_tilde(int i, char **str_add)
{
	char	*str;
	char	*str_start;
	char	*str_end;
	char	*expand;

	str = NULL;
	str_sart = NULL;
	str_end = NULL:
	str = *str_add;
}
*/