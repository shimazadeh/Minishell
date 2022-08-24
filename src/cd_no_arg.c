/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_no_arg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 21:05:30 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/24 21:05:45 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd_no_arg(char **dir_add, t_list **envp_head)
{
	char	*dir;
	char	*home_exp;

	dir = NULL;
	home_exp = NULL;
	find_env_var("HOME", envp_head, &home_exp);
	if (home_exp)
	{
		dir = ft_strdup(home_exp);
		ft_free(home_exp);
	}
	else
		dir = ft_getcwd();
	if (dir)
		*dir_add = dir;
}
