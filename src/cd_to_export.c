/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_to_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 21:11:27 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/29 17:23:20 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd_to_export_1(char *curpath, char *pwd_exp, t_list **envp_head)
{
	int		ret1;
	int		ret2;
	char	**to_export;

	ret1 = 0;
	ret2 = 0;
	to_export = NULL;
	if (cd_to_export_2(&to_export, envp_head, "PWD", curpath))
			ret1 = export(to_export, envp_head);
	ft_free(curpath);
	ft_free(to_export);
	to_export = NULL;
	to_export = ft_alloc(sizeof(char *) * 2);
	if (cd_to_export_2(&to_export, envp_head, "OLDPWD", pwd_exp))
		ret2 = export(to_export, envp_head);
	ft_free(pwd_exp);
	ft_free(to_export);
	if (!ret2)
		return (ret1);
	return (ret2);
}

int	cd_to_export_2(char ***to_export_add, t_list **envp_head, \
char *var_name, char *var_exp)
{
	char	*null_add;
	char	**to_export;
	char	*var_name_eq;

	null_add = NULL;
	to_export = NULL;
	var_name_eq = NULL;
	if (!find_env_var(var_name, envp_head, &null_add))
		return (0);
	if (null_add)
		ft_free(null_add);
	to_export = ft_alloc(sizeof(char *) * 2);
	var_name_eq = ft_strjoin(var_name, "=");
	to_export[0] = ft_strjoin(var_name_eq, var_exp);
	ft_free(var_name_eq);
	to_export[1] = NULL;
	*to_export_add = to_export;
	return (1);
}
