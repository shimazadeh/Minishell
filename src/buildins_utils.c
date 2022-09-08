/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 20:50:57 by aguillar          #+#    #+#             */
/*   Updated: 2022/09/08 21:16:01 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	contains_invalid_char(char *str, char *id, int j)
{
	int	i;

	i = 0;
	if (str[0] == '=')
		return (1);
	while (str[i] && (!j || str[i] != '='))
	{
		if (ft_strchr(id, str[i]))
			return (1);
		i++;
	}
	return (0);
}

int	find_env_var(char *var_name, t_list **envp_head, char **var_exp_add)
{
	int		s;
	char	*var_exp;
	t_list	*node;

	s = 0;
	var_exp = NULL;
	node = NULL;
	if (!var_name || !envp_head || !var_exp_add)
		ft_exit(EXIT_FAILURE, "find_env_var", "argument check fail");
	node = *envp_head;
	s = ft_strlen(var_name);
	while (node)
	{
		if (!ft_strncmp(var_name, (char *)node->content, s))
		{
			if ((char)(((char *)(node->content))[s] == '=')
				&& (char)(((char *)(node->content))[s + 1]))
				var_exp = ft_strdup((char *)(&((char *)(node->content))[++s]));
			*var_exp_add = var_exp;
			return (1);
		}
		node = node->next;
	}
	*var_exp_add = var_exp;
	return (0);
}

char	*ft_getcwd(void)
{
	char	*pwd;
	char	*tmp;

	pwd = NULL;
	tmp = getcwd(NULL, 0);
	if (!tmp)
		ft_exit(errno, "ft_getpwd", "getcwd fail");
	pwd = ft_strdup(tmp);
	free(tmp);
	return (pwd);
}
