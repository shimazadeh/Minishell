/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 16:06:28 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/10 20:34:45 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_user(t_list **envp_head)
{
	int		i;
	char	*user;
	t_list	*node;

	if (!envp_head || !*envp_head)
		return (NULL);
	node = *envp_head;
	while (node && ft_strncmp((char *)node->content, "USER=", 5))
		node = node->next;
	if (node)
	{
		user = ft_strndup((char *)(&node->content)[5], ft_strlen((char *)(&node->content)[5]));
		if (!user)
			ft_exit(EXIT_FAILURE, NULL);
	}
	return (user);
}

char	*get_pwd(t_list **envp_head, char *user)
{
	t_list	*node;
	char	*pwd;
	char	*usr_in_pwd;

	if (!user || !envp_head || !*envp_head)
		return (NULL);
	node = *envp_head;
	while (node && ft_strncmp((char *)node->content, "PWD=", 4))
		node = node->next;
	if (node)
	{
		usr_in_pwd = ft_strstr((char *)(&node->content)[4], user);
		if (usr_in_pwd)
		{
			pwd = ft_strndup(&usr_in_pwd[ft_strlen(user) - 1], ft_strlen(&usr_in_pwd[ft_strlen(user) - 1]));
			if (!pwd)
				ft_exit(EXIT_FAILURE, NULL);
			pwd[0] = '~';
		}
		else
		{
			pwd = ft_strndup((char *)(&node->content)[4], ft_strlen((char *)(&node->content)[4]));
			if (!pwd)
				ft_exit(EXIT_FAILURE, NULL);
		}
	}
	return (pwd);
}

char	*get_pos(t_list **envp_head)
{
	t_list	*node;
	int		j;
	char	*pos;
	char	*str;

	if (!envp_head || !*envp_head)
		return (NULL);
	node = *envp_head;
	while (node && ft_strncmp((char *)node->content, "SESSION_MANAGER=local/", 22))
		node = node->next;
	if (!node)
		return (NULL);
	j = 0;
	str = (char *)node->content;
	while (str[23 + j] && str[22 + j] != '.')
		j++;
	pos = ft_strndup(&str[22], j);
	if (!pos)
		ft_exit(EXIT_FAILURE, NULL);
	return (pos);
}

char	**set_prompt_elems(t_list **envp_head)
{
	char	**prompt_elems;
	int		i;

	prompt_elems = ft_alloc(sizeof(char *) * 7);
	if (!prompt_elems)
		ft_exit(EXIT_FAILURE, NULL);
	prompt_elems[0] = get_user(envp_head);
	prompt_elems[1] = ft_strndup("@", 1);
	prompt_elems[2] = get_pos(envp_head);
	prompt_elems[3] = ft_strndup(":", 1);
	prompt_elems[4] = get_pwd(envp_head, prompt_elems[0]);
	prompt_elems[5] = ft_strndup("$ ", 2);
	prompt_elems[6] = NULL;
	i = 0;
	while (i < 6)
	{
		if (!prompt_elems[i])
		{
			free_tab_n(prompt_elems, 6);
			return (NULL);
		}
		i++;
	}
	return (prompt_elems);
}

void	get_prompt(char **prompt_add, t_list **envp_head)
{
	char	**prompt_elems;
	char	*tmp;
	int		i;

	prompt_elems = set_prompt_elems(envp_head);
	if (!prompt_elems)
	{
		*prompt_add = ft_strndup("$>", 2);
		if (!*prompt_add)
			ft_exit(EXIT_FAILURE, NULL);
		return ;
	}
	tmp = NULL;
	i = 0;
	while (prompt_elems[i])
	{
		tmp = *prompt_add;
		*prompt_add = ft_strjoin(*prompt_add, prompt_elems[i]);
		if (!*prompt_add)
			ft_exit(EXIT_FAILURE, NULL);
		ft_free(tmp);
		i++;
	}
	free_tab_n(prompt_elems, 6);
}
