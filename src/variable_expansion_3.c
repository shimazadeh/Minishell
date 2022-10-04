/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_3.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 00:29:19 by shabibol          #+#    #+#             */
/*   Updated: 2022/10/04 14:10:55 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_last_ec(char *str, int *i, int last_ec)
{
	char	*var_exp;
	char	*new_str;

	var_exp = ft_itoa(last_ec);
	new_str = create_new_str(str, var_exp, i, *i + 2);
	ft_free(str);
	return (new_str);
}

int	var_expand_size(char *str)
{
	int	i;
	int	size;

	i = 0;
	size = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			i += go_to_closing_char(&str[i]) + 1;
			size++;
		}
		else
		{
			size++;
			while (str[i] && str[i] != '\'' && str[i] != '\"')
				i++;
		}
	}
	return (size);
}
