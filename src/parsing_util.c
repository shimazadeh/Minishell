/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shabibol <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/10 00:07:46 by shabibol          #+#    #+#             */
/*   Updated: 2022/09/10 00:07:47 by shabibol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	quotes_presence(char *str)
{
	int	i;

	i = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	while (str[i] && !end_char(str[i], "> \t<"))
	{
		if (str[i] == '\'' || str[i] == '\"')
			return (1);
		i++;
	}
	return (0);
}

int	move_the_char_back(char *str)
{
	int		i;
	char	c;

	i = 0;
	while (str[i + 1])
	{
		c = str[i + 1];
		str[i] = c;
		i++;
	}
	while (str[i])
	{
		str[i] = '\0';
		i++;
	}
	return (0);
}
