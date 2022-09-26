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

char	*replace_with_space(char *str, int start, int end)
{
	if (!str || start == end)
		return (str);
	while (str[start] && start < end)
	{
		str[start] = ' ';
		start++;
	}
	return (str);
}

int	number_of_delim(char *str, char delim, int flag)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	if (!str || !delim)
		return (0);
	while (str[i])
	{
		if (str[i] == '\"' || str[i] == '\'')
			i = i + go_to_closing_char(&str[i]);
		else if (flag == 0 && str[i] == delim && str[i + 1] != delim)
			count++;
		else if (flag == 0 && str[i] == delim && str[i + 1] == delim)
			i++;
		else if (flag == 1 && str[i] == delim && str[i + 1] == delim)
		{
			count++;
			i++;
		}
		else if (flag == 1 && str[i] == delim && str[i + 1] != delim)
			count++;
		i++;
	}
	return (count);
}

int	supress_the_quotes(char **str_add, int i)
{
	int		k;
	char	*str;

	str = *str_add;
	k = i + go_to_closing_char(&str[i]) - 1;
	move_the_char_back(&str[i]);
	move_the_char_back(&str[k]);
	*str_add = str;
	return (k);
}
