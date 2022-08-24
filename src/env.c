/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 13:55:57 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/24 13:56:04 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env(t_list **envp_head)
{
	t_list	*node;

	if (!envp_head || !*envp_head)
		ft_exit(EXIT_FAILURE, "Exited in function: env\nExit due to: argument check fail\n");
	node = *envp_head;
	while (node)
	{
		if (ft_strchr((char *)node->content, '='))
			ft_dprintf(1, "%s\n", (char *)node->content);
		node = node->next;
	}
	return (EXIT_SUCCESS);
}
