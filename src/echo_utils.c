/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 14:31:00 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/25 01:53:05 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tab_nl(char **tab, int nl)
{
	int	i;

	i = 0;
	if (!tab)
		ft_exit(EXIT_FAILURE, "print_tab_nl", "argument check fail");
	while (tab[i])
	{
		ft_dprintf(1, "%s", tab[i]);
		if (tab[i + 1])
			ft_dprintf(1, " ", tab[i]);
		i++;
	}
	if (nl)
		ft_dprintf(1, "\n", tab[i]);
}
