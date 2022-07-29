/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 16:06:28 by aguillar          #+#    #+#             */
/*   Updated: 2022/07/24 17:26:04 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strstr(char *str, char *to_find)
{
	int	i;
	int	j;

	i = 0;
	if (!to_find[i])
		return (str);
	while (str[i])
	{
		j = 0;
		while (str[i + j] == to_find[j])
		{
			if (!to_find[j + 1])
				return (&str[i]);
			j++;
		}
		i++;
	}
	return (0);
}

char	*get_user(char **envp)
{
	int		i;
	char	*user;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "USER=", 5) != 0)
		i++;
	if (envp[i])
		user = ft_strndup(&envp[i][5], ft_strlen(&envp[i][5]));
	return (user);
}

char	*get_pwd(char **envp, char *user)
{
	int		i;
	char	*pwd;
	char	*usr_in_pwd;

	if (!user)
		return (NULL);
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PWD=", 4) != 0)
		i++;
	if (envp[i])
	{
		usr_in_pwd = ft_strstr(&envp[i][4], user);
		if (usr_in_pwd)
		{
			pwd = ft_strndup(&usr_in_pwd[ft_strlen(user) - 1], ft_strlen(&usr_in_pwd[ft_strlen(user) - 1]));
			pwd[0] = '~';
		}
		else
			pwd = ft_strndup(&envp[i][4], ft_strlen(&envp[i][4]));
	}
	return (pwd);
}

char	*get_pos(char **envp)
{
	int		i;
	int		j;
	char	*pos;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "SESSION_MANAGER=local/", 22) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	while (envp[i][23 + j] && envp[i][22 + j] != '.')
		j++;
	pos = ft_strndup(&envp[i][22], j);
	return (pos);
}

void	free_tab_n(char **tab, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		if (tab[i])
			free(tab[i]);
		i++;
	}
	free(tab);
}

char	**set_prompt_elems(char **envp)
{
	char	**prompt_elems;
	int		i;

	prompt_elems = malloc(sizeof(char *) * 7);
	prompt_elems[0] = get_user(envp);
	prompt_elems[1] = ft_strndup("@", 1);
	prompt_elems[2] = get_pos(envp);
	prompt_elems[3] = ft_strndup(":", 1);
	prompt_elems[4] = get_pwd(envp, prompt_elems[0]);
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

char	*get_prompt(char **envp)
{
	char	**prompt_elems;
	char	*prompt;
	char	*tmp;
	int		i;

	prompt_elems = set_prompt_elems(envp);
	if (!prompt_elems)
		return (NULL);
	prompt = NULL;
	tmp = NULL;
	i = 0;
	while (prompt_elems[i])
	{
		tmp = prompt;
		prompt = ft_strjoin(prompt, prompt_elems[i]);
		if (!prompt)
			return (NULL);
		free(tmp);
		i++;
	}
	free_tab_n(prompt_elems, 6);
	return (prompt);
}