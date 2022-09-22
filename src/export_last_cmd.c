/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_last_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 14:05:15 by aguillar          #+#    #+#             */
/*   Updated: 2022/09/22 18:00:07 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	export_last_cmd(char **cmd, t_list **envp)
{
	int		i;
	char	*to_export[2];

	i = 0;
	to_export[1] = NULL;
	while (cmd[i] && cmd[i + 1])
		i++;
	to_export[0] = ft_strjoin("_=", cmd[i]);
	export(to_export, envp);
	ft_free(to_export[0]);
}

void	export_next_cmd(char *cmd, t_list **envp)
{
	char	*to_export[2];

	to_export[1] = NULL;
	to_export[0] = ft_strjoin("_=", cmd);
	export(to_export, envp);
	ft_free(to_export[0]);
}
