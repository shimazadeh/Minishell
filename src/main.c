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

//exit fix
// ws + unclosed auotes
//tile expand

t_list	*g_alloc_lst = NULL;

int	main(int ac, char **av, char **envp)
{
	(void)av;
	if (ac != 1)
		ft_exit(EXIT_FAILURE, "main", "invalid number of argument");
	minishell(envp);
	return (0);
}
