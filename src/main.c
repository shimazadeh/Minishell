/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 00:18:40 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/25 01:01:37 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_global	*g_var = NULL;

int	main(int ac, char **av, char **envp)
{
	(void)av;
	g_var = malloc(sizeof(t_global));
	g_var->sig_flag = 0;
	g_var->g_alloc_lst = NULL;
	if (ac != 1)
		ft_exit(EXIT_FAILURE, "main", "invalid number of argument");
	minishell(envp);
	free(g_var);
	return (0);
}

/*  in use at exit: 204,728 bytes in 216 blocks
==92700==   total heap usage: 956 allocs, 740 frees, 242,334 bytes allocated*/
