/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 18:56:33 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/25 00:58:09 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit(int exit_code, char *fn_name, char *error_msg)
{
	int	fd;

	fd = 1;
	if (exit_code)
		fd = 2;
	if (fn_name)
		dprintf(fd, "Exited in function: %s\n", fn_name);
	if (error_msg)
		ft_dprintf(fd, "Exit due to: %s\n", error_msg);
	rl_clear_history();
	if (g_alloc_lst)
		ft_free_list_regular(g_alloc_lst);
	exit(exit_code);
}
