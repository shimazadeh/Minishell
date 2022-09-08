/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 14:33:09 by aguillar          #+#    #+#             */
/*   Updated: 2022/09/08 21:13:13 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset_init_vars(t_unset_vars v[1])
{
	v->node = NULL;
	v->tmp = NULL;
	v->ret = 0;
}

int	unset(char **vars, t_list **envp_head)
{
	int				i;
	t_unset_vars	v[1];

	i = 0;
	unset_init_vars(v);
	if (!envp_head || !vars)
		ft_exit(EXIT_FAILURE, "unset", "argument check fail");
	if (!vars[0] || !*envp_head)
		return (v->ret);
	i = 0;
	while (vars[i])
	{
		if (contains_invalid_char(vars[i], "@#$%^&*-+=<>|\\", 0))
		{
			ft_dprintf(2, "bash: unset: `%s': not a valid identifier\n", \
				vars[i]);
			v->ret = EXIT_FAILURE;
		}
		else
			unset_exec(i, vars, envp_head, v);
		i++;
	}
	return (v->ret);
}

void	unset_exec(int i, char **vars, t_list **envp_head, t_unset_vars v[1])
{
	v->node = *envp_head;
	while (v->node)
	{
		v->tmp = v->node->next;
		if (!ft_strncmp(vars[i], (char *)v->node->content, ft_strlen(vars[i])))
			ft_list_remove_node(envp_head, v->node);
		v->node = v->tmp;
	}
}
