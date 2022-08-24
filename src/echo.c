/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 14:28:07 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/25 01:44:24 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo(char **av)
{
	if (!av)
		ft_exit(EXIT_FAILURE, "echo", "argument check fail");
	else if (av[0] && !ft_strncmp(av[0], "-n", 3))
		print_tab_nl(&av[1], 0);
	else
		print_tab_nl(av, 1);
	return (EXIT_SUCCESS);
}
