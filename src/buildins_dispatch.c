/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildins_dispatch.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/08 16:25:55 by aguillar          #+#    #+#             */
/*   Updated: 2022/09/20 15:00:49 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	buildins_dispatch(char **av, t_list **envp_head)
{
	if (!av || !av[0])
		ft_exit(EXIT_FAILURE, "buildins_dispatch", "argument check fail");
	else if (!ft_strncmp(av[0], "echo", 5))
		return (echo(&av[1]));
	else if (!ft_strncmp(av[0], "cd", 3))
		return (cd(&av[1], envp_head));
	else if (!ft_strncmp(av[0], "pwd", 4))
		return (pwd());
	else if (!ft_strncmp(av[0], "export", 7))
		return (export(&av[1], envp_head));
	else if (!ft_strncmp(av[0], "unset", 6))
		return (unset(&av[1], envp_head));
	else if (!ft_strncmp(av[0], "env", 4) && !av[1])
		return (env(envp_head));
	else if (!ft_strncmp(av[0], "exit", 5))
		ft_exit(EXIT_SUCCESS, NULL, NULL);
	return (127);
}

int	boolean_if_buildin(char **av)
{
	if (!av || !av[0])
		return (-1);
	if (!ft_strncmp(av[0], "echo", 5))
		return (1);
	if (!ft_strncmp(av[0], "cd", 3))
		return (1);
	if (!ft_strncmp(av[0], "pwd", 4))
		return (1);
	if (!ft_strncmp(av[0], "export", 7))
		return (1);
	if (!ft_strncmp(av[0], "unset", 6))
		return (1);
	if (!ft_strncmp(av[0], "env", 4) && !av[1])
		return (1);
	if (!ft_strncmp(av[0], "exit", 5))
		return (1);
	return (0);
}
