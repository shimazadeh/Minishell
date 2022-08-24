/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_to_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 21:11:27 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/24 21:11:34 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd_to_export(char *curpath, char *pwd_exp, t_list **envp_head)
{
	int		ret1;
	int		ret2;
	char	**to_export;

	ret1 = 0;
	ret2 = 0;
	to_export = NULL;
	to_export = ft_alloc(sizeof(char *) * 2);
	to_export[0] = ft_strjoin("PWD=", curpath);
	to_export[1] = NULL;
	ft_free(curpath);
	ret1 = export(to_export, envp_head);
	ft_free(to_export);
	to_export = ft_alloc(sizeof(char *) * 2);
	to_export[0] = ft_strjoin("OLDPWD=", pwd_exp);
	to_export[1] = NULL;
	ft_free(pwd_exp);
	ret2 = export(to_export, envp_head);
	ft_free(to_export);
	if (!ret2)
		return (ret1);
	return (ret2);
}
