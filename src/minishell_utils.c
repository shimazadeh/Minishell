/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 12:43:57 by aguillar          #+#    #+#             */
/*   Updated: 2022/09/08 21:22:42 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	even_par_nbr(char *str)
{
	int	i;
	int	par;

	i = 0;
	par = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			i += go_to_closing_char(&str[i]);
		else if (str[i] == '(')
			par++;
		else if (str[i] == ')')
			par--;
		i++;
	}
	if (par)
	{
		ft_dprintf(2, "Error: input contains odd number of parentheses!\n");
		return (0);
	}
	return (1);
}

int	no_unclosed_quote(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			j = go_to_closing_char(&str[i]);
			if (!j)
			{
				ft_dprintf(2, \
					"Error: quote '%c' at index %d is unclosed!\n", str[i], i);
				return (0);
			}
			i += j;
		}
		i++;
	}
	return (1);
}

void	handle_ws(char **str_add)
{
	int		size;
	char	*new_str;
	char	*str;

	size = 0;
	str = NULL;
	new_str = NULL;
	str = *str_add;
	handle_ws_get_str_size(&size, str);
	new_str = ft_alloc(sizeof(char) * (size + 1));
	ft_bzero(new_str, sizeof(char) * (size + 1));
	handle_ws_fill_str(str, new_str);
	ft_free(str);
	*str_add = new_str;
}

void	handle_ws_get_str_size(int *size_add, char *str)
{
	int	gtcc;
	int	i;
	int	size;

	gtcc = 0;
	i = 0;
	size = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			gtcc = go_to_closing_char(&str[i]);
			i += gtcc + 1;
			size += gtcc + 1;
		}
		else if (str[i] == ' ' || str[i] == '\t')
		{
			size += 1;
			while (str[i] == ' ' || str[i] == '\t')
				i++;
		}
		else if (++i)
			size++;
	}
	*size_add = size;
}

void	handle_ws_fill_str(char *str, char *new_str)
{
	int		gtcc;
	int		i;
	int		j;

	gtcc = 0;
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			gtcc = go_to_closing_char(&str[i]);
			while (gtcc-- >= 0)
				new_str[j++] = str[i++];
		}
		else if (str[i] == ' ' || str[i] == '\t')
		{
			new_str[j] = ' ';
			j++;
			while (str[i] == ' ' || str[i] == '\t')
				i++;
		}
		else
			new_str[j++] = str[i++];
	}
}
