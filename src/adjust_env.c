/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adjust_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 15:14:28 by aguillar          #+#    #+#             */
/*   Updated: 2022/09/23 17:39:52 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	adjust_env(t_list **envp_head)
{
	char	*to_export[2];
	char	*pwd;
	char	*shlvl;
	char	*new_shlvl;

	shlvl = NULL;
	pwd = ft_getcwd();
	to_export[1] = NULL;
	to_export[0] = ft_strjoin("PWD=", pwd);
	export(to_export, envp_head);
	ft_free(to_export[0]);
	find_env_var("SHLVL", envp_head, &shlvl);
	if (!shlvl || contains_non_digit(shlvl))
		to_export[0] = ft_strdup("SHLVL=0");
	else if (shlvl_too_high(shlvl))
		to_export[0] = ft_strdup("SHLVL=1");
	else
	{
		new_shlvl = ft_itoa(ft_atoi(shlvl) + 1);
		to_export[0] = ft_strjoin("SHLVL=", new_shlvl);
		ft_free(new_shlvl);
	}
	export(to_export, envp_head);
	ft_free(shlvl);
	adjust_env_underscore(to_export, pwd, envp_head);
}

void	adjust_env_underscore(char *to_export[2], char *pwd, t_list **envp_head)
{
	char	*underscore;

	underscore = NULL;
	ft_free(to_export[0]);
	find_env_var("_", envp_head, &underscore);
	if (!underscore)
	{
		underscore = ft_strjoin(pwd, "/minishell");
		to_export[0] = ft_strjoin("_=", underscore);
		export(to_export, envp_head);
		ft_free(underscore);
		ft_free(to_export[0]);
	}
	ft_free(pwd);
}

int	contains_non_digit(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] > '9' || str[i] < '0')
			return (1);
		i++;
	}
	return (0);
}

int	shlvl_too_high(char *shlvl)
{
	int		new_shlvl;
	char	*new_shlvl_str;

	if (!shlvl)
		return (0);
	if (ft_strlen(shlvl) >= 10 && ft_strncmp(shlvl, "2147483647", 10) >= 0)
	{
		ft_dprintf(1, SHLVL_ERR, "above INT MAX");
		return (1);
	}
	new_shlvl = ft_atoi(shlvl) + 1;
	if (new_shlvl > 999)
	{
		new_shlvl_str = ft_itoa(new_shlvl);
		ft_dprintf(1, SHLVL_ERR, new_shlvl_str);
		ft_free(new_shlvl_str);
		return (1);
	}
	return (0);
}
