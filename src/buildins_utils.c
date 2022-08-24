/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 20:50:57 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/25 01:39:16 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	contains_invalid_char(char *str, char *id, int j)
{
	int	i;

	i = 0;
	while (id[i])
	{
		if (ft_strchr(str, id[i]) && !(j == 1 && i && ft_strchr(str, '=')))
			return (1);
		i++;
	}
	return (0);
}

void	find_env_var(char *var_name, t_list **envp_head, char **var_exp_add)
{
	int		size;
	char	*var_exp;
	char	*var_name_eq;
	t_list	*node;

	size = 0;
	var_exp = NULL;
	var_name_eq = NULL;
	node = NULL;
	if (!var_name || !envp_head || !var_exp_add)
		ft_exit(EXIT_FAILURE, "find_env_var", "argument check fail");
	node = *envp_head;
	var_name_eq = ft_strjoin(var_name, "=");
	size = ft_strlen(var_name_eq);
	while (node)
	{
		if (!ft_strncmp(var_name_eq, (char *)node->content, size) \
			&& (char)(((char *)(node->content))[size]))
			var_exp = ft_strdup((char *)(&((char *)(node->content))[size]));
		node = node->next;
	}
	*var_exp_add = var_exp;
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
