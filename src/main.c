/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 00:18:40 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/24 00:33:25 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*g_alloc_lst = NULL;

int	main(int ac, char **av, char **envp)
{
	(void)av;
	if (ac != 1)
		ft_exit(EXIT_FAILURE,
			"Exited in function: main\nExit due to: argument check fail\n");
	minishell(envp);
	return (0);
}
