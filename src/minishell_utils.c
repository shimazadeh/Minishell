/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 12:43:57 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/24 19:57:13 by aguillar         ###   ########.fr       */
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

int	no_unclosed_quote(char *str)
{
	(void)str;
	return (1);
}

void	handle_ws(char **str_add)
{
	(void)str_add;
}
