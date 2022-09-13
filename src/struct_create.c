/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_utils_lst.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/28 01:15:16 by shabibol          #+#    #+#             */
/*   Updated: 2022/08/22 22:16:35 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	structure_add_back(t_struct **sc, t_struct *new)
{
	t_struct	*repl;

	if (!new)
		return ;
	if (!(*sc))
	{
		*sc = new;
		return ;
	}
	repl = structure_last(*sc);
	repl->next = new;
}

t_struct	*structure_last(t_struct	*sc)
{
	if (!sc)
		return (0);
	while (sc->next)
		sc = sc->next;
	return (sc);
}

int	struct_size(t_struct *sc)
{
	int	i;

	i = 0;
	while (sc)
	{
		sc = sc->next;
		i++;
	}
	return (i);
}
