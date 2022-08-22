/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 18:56:33 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/22 01:43:41 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit(int exit_code, char *error_msg)
{
	int	fd;

	fd = 1;
	if (exit_code)
		fd = 2;
	if (error_msg)
		ft_dprintf(fd, "%s", error_msg);
	rl_clear_history();
	if (alloc_lst)
		ft_free_list_regular(alloc_lst);
	exit(exit_code);
}
