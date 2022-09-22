/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 13:56:40 by aguillar          #+#    #+#             */
/*   Updated: 2022/09/22 17:42:24 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	export(char **vars, t_list **envp_head)
{
	if (!envp_head || !vars)
		ft_exit(EXIT_FAILURE, "export", "argument check fail");
	if (!vars[0])
		return (export_no_arg(envp_head));
	return (export_arg(0, 0, vars, envp_head));
}

int	export_no_arg(t_list **envp_head)
{
	print_sorted_list(envp_head);
	return (EXIT_SUCCESS);
}

int	export_arg(int i, int ret, char **vars, t_list **envp_head)
{
	t_list	*old;

	old = NULL;
	while (vars[i])
	{
		if (contains_invalid_char(vars[i], "@#$%^&*-+=<>|\\", 1))
		{
			ft_dprintf(2, "bash: export: `%s': not a valid identifier\n",
				vars[i]);
			ret = EXIT_FAILURE;
		}
		else
		{
			old = old_var(vars[i], envp_head);
			if (old)
			{
				ft_free(old->content);
				old->content = ft_strdup(vars[i]);
			}
			else
				ft_lstadd_back(envp_head, ft_lstnew(ft_strdup(vars[i])));
		}
		i++;
	}
	return (ret);
}
