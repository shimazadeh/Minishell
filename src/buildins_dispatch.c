/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildins_dispatch.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/08 16:25:55 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/24 14:33:31 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	buildins_dispatch(char **av, t_list **envp_head)
{
	if (!av || !av[0])
		ft_exit(EXIT_FAILURE, "Exited in function: buildins_dispatch\nExit due to: argument check fail\n");
	else if (!ft_strncmp(av[0], "echo", 5))
		return (echo(&av[1]));
	else if (!ft_strncmp(av[0], "cd", 3) && (!av[1] || (av[1] && !av[2])))
		return (cd(av[1], envp_head));
	else if (!ft_strncmp(av[0], "pwd", 4))
		return (pwd());
	else if (!ft_strncmp(av[0], "export", 7))
		return (export(&av[1], envp_head));
	else if (!ft_strncmp(av[0], "unset", 6))
		return (unset(&av[1], envp_head));
	else if (!ft_strncmp(av[0], "env", 4) && !av[1])
		return (env(envp_head));
	else if (!ft_strncmp(av[0], "exit", 5))
		ft_exit(EXIT_SUCCESS, NULL);
	return (127);
}
