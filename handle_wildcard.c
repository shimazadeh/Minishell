/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 15:43:13 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/19 16:22:04 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_wildcard(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != ' ')
	{

		i++;
	}
}

void	expand_next_wildcard(char **str_add char *wc)
{
	char	*str;
	char	*str_pre_wc;
	char	*str_post_wc;
	char	*expand;
	char	*tmp;
	int		i;

	str = wc;
	str_pre_wc = NULL;
	str_post_wc = NULL;
	expand = NULL;
	i = 0;
	while (i > 0 && str[i - 1] != ' ')
		i--;
	expand = expand_wildcard(&str[i]);
	if (i)
	{
		str[i] = '\0';
		str_pre_wc = *str_add;
	}
	i = 0;
	while (str[i] && str[i] != ' ')
		i++;
	if (str[i])
		str_post_wc = &str[i];
	str = ft_strjoin(str_pre_wc, expand);
	if (!str)
		ft_exit(errno, NULL);
	tmp = str;
	str = ft_strjoin(tmp, str_post_wc);
	if (!str)
		ft_exit(errno, NULL);
	ft_free(tmp);
	ft_free(*str_add);
	*str_add = str;
}

void	handle_wildcards(char **str_add)
{
	char	*str;
	char	*wc;
	int		flag;

	str = *str_add;
	flag = 1;
	while (flag == 1 || wc)
	{
		flag = 0;
		wc = ft_strchr(str, '*');
		expand_next_wildcard(str_add, wc);
	}
}