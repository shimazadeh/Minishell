/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 16:06:28 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/20 04:28:02 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_user(t_list **envp_head)
{
	char	*user;
	t_list	*node;

	if (!envp_head || !*envp_head)
		return (NULL);
	user = NULL;
	node = *envp_head;
	while (node && ft_strncmp((char *)node->content, "USER=", 5))
		node = node->next;
	if (node)
	{
		user = ft_strdup(node->content + (sizeof(char) * 5));
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
	pwd = NULL;
	usr_in_pwd = NULL;
	while (node && ft_strncmp((char *)node->content, "PWD=", 4))
		node = node->next;
	if (node)
	{
		usr_in_pwd = ft_strstr((node->content + (sizeof(char) * 4)), user);
		if (usr_in_pwd)
		{
			pwd = ft_strdup(&usr_in_pwd[ft_strlen(user) - 1]);
			if (!pwd)
				ft_exit(EXIT_FAILURE, NULL);
			pwd[0] = '~';
		}
		else
		{
			pwd = ft_strdup((node->content + (sizeof(char) * 4)));
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
	j = 0;
	pos = NULL;
	str = NULL;
	while (node && ft_strncmp((char *)node->content, "SESSION_MANAGER=local/", 22))
		node = node->next;
	if (!node)
		return (NULL);
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
	prompt_elems[1] = ft_strdup("@");
	prompt_elems[2] = get_pos(envp_head);
	prompt_elems[3] = ft_strdup(":");
	prompt_elems[4] = get_pwd(envp_head, prompt_elems[0]);
	prompt_elems[5] = ft_strdup("$ ");
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
	char	*prompt;
	char	*tmp;
	int		i;

	prompt_elems = NULL;
	prompt = NULL;
	tmp = NULL;
	i = 0;
	prompt_elems = set_prompt_elems(envp_head);
	if (!prompt_elems)
	{
		*prompt_add = ft_strdup("$> ");
		if (!*prompt_add)
			ft_exit(EXIT_FAILURE, NULL);
		return ;
	}
	while (prompt_elems[i])
	{
		tmp = prompt;
		prompt = ft_strjoin(tmp, prompt_elems[i]);
		if (!prompt)
			ft_exit(EXIT_FAILURE, NULL);
		ft_free(tmp);
		i++;
	}
	*prompt_add = prompt;
	free_tab_n(prompt_elems, 6);
}
