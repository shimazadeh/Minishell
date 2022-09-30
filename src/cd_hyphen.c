/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_hyphen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 21:08:19 by aguillar          #+#    #+#             */
/*   Updated: 2022/09/30 17:16:26 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd_hyphen(t_list **envp_head)
{
	int		ret;
	char	*oldpwd_exp;
	char	*to_cd[2];

	ret = 0;
	oldpwd_exp = NULL;
	find_env_var("OLDPWD", envp_head, &oldpwd_exp);
	if (!oldpwd_exp)
	{
		ft_dprintf(2, "bash: cd: OLDPWD not set\n");
		return (EXIT_FAILURE);
	}
	to_cd[0] = oldpwd_exp;
	to_cd[1] = NULL;
	ret = cd(to_cd, envp_head);
	ft_free(oldpwd_exp);
	if (!ret)
		return (pwd());
	return (ret);
}
