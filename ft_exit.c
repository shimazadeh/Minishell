/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 18:56:33 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/11 19:23:25 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit(int exit_code, char *error_msg)
{
	if (error_msg)
		ft_dprintf(2, "%s", error_msg);
	if (alloc_lst)
		ft_free_list_regular(*alloc_lst);
	exit(exit_code);
}
