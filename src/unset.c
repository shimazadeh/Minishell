/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 14:33:09 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/24 14:33:26 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset(char **vars, t_list **envp_head)
{
	t_list	*node;
	t_list	*tmp;
	int		i;
	int		ret;

	if (!envp_head || !vars)
		ft_exit(EXIT_FAILURE, "Exited in function: unset\nExit due to: argument check fail\n");
	ret = 0;
	if (!vars[0] || !*envp_head)
		return (ret);
	i = 0;
	while (vars[i])
	{
		if (contains_invalid_char(vars[i], "@#$%^&*-+=_<>|\\", 0))
		{
			ft_dprintf(2, "bash: unset: `%s': not a valid identifier\n", vars[i]);
			ret = EXIT_FAILURE;
		}
		else
		{
			node = *envp_head;
			while (node)
			{
				tmp = node->next;
				if (!ft_strncmp(vars[i], (char *)node->content, ft_strlen(vars[i])))
					ft_list_remove_node(envp_head, node);
				node = tmp;
			}
		}
		i++;
	}
	return (ret);
}
