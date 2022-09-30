/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 18:56:33 by aguillar          #+#    #+#             */
/*   Updated: 2022/09/21 16:11:46 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit(char **av, int exit_code, char *fn_name, char *error_msg)
{
	int	fd;

	exit_code = exit_check_args(av, exit_code);
	fd = 1;
	if (exit_code)
		fd = 2;
	if (fn_name)
		ft_dprintf(fd, "Exited in function: %s\n", fn_name);
	if (error_msg)
		ft_dprintf(fd, "Exit due to: %s\n", error_msg);
	rl_clear_history();
	if (g_var->g_alloc_lst)
		ft_free_list_regular(g_var->g_alloc_lst);
	free(g_var);
	exit(exit_code);
}

int	exit_check_args(char **av, int exit_code)
{
	int	i;
	int	ret;

	ret = 0;
	i = -1;
	if (!av || !av[0])
		ret = exit_code % 255;
	else if (av[0])
	{
		if (!av[0][0])
			ret = 2;
		while (av[0][++i])
			if (av[0][i] < '0' || av[0][i] > '9')
				ret = 2;
		if (ret == 2)
			ft_dprintf(1, "bash: exit: : numeric argument required\n");
		if (!ret)
			ret = ft_atoi(av[0]) % 256;
	}
	if (ret != 2 && av && av[0] && av[1])
	{
		ret = 1;
		ft_dprintf(1, "bash: exit: too many arguments\n");
	}
	return (ret);
}
