/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_curpath_no_abs_path.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 21:09:31 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/24 21:09:53 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd_curpath_no_abs_path(char **curpath_add, char *pwd_exp)
{
	char	*curpath;
	char	*tmp;

	curpath = NULL;
	tmp = NULL;
	curpath = *curpath_add;
	if ((pwd_exp[ft_strlen(pwd_exp) - 1] != '/'))
	{
		tmp = ft_strdup(pwd_exp);
		pwd_exp = ft_strjoin(tmp, "/");
		ft_free(tmp);
	}
	tmp = curpath;
	curpath = (ft_strjoin(pwd_exp, tmp));
	ft_free(tmp);
	*curpath_add = curpath;
}
