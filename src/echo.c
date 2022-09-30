/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 14:28:07 by aguillar          #+#    #+#             */
/*   Updated: 2022/09/30 16:40:46 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo(char **av)
{
	int	i;

	i = 1;
	if (!av)
		ft_exit(EXIT_FAILURE, "echo", "argument check fail");
	else if (is_echo_option(av[0]))
	{
		while (is_echo_option(av[i]))
			i++;
		print_tab_nl(&av[i], 0);
	}
	else
		print_tab_nl(av, 1);
	return (EXIT_SUCCESS);
}

int	is_echo_option(char *str)
{
	int	i;

	i = 1;
	if (!str)
		return (0);
	if (str[0] != '-')
		return (0);
	if (!str[1])
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}
