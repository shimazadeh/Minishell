/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 13:54:56 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/24 13:55:09 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd(void)
{
	char	*pwd;

	pwd = ft_getcwd();
	if (pwd)
	{
		ft_dprintf(1, "%s\n", pwd);
		ft_free(pwd);
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}
