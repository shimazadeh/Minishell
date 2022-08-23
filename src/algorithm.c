/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithm.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 14:27:04 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/24 00:37:50 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



int	algorithm(char *str, t_list **envp_head, int last_exit_code)
{
	int 	i;
	int 	j;
	int		k;
	int		pipex_ret;
	int		ret;
	char	*tmp_str;
	t_list	*sep;
	t_list	*submod;
 	t_list	*sep_head;
 	t_list	*submod_head;

	i = 0;
	j = 0;
	k = 0;
	pipex_ret = 0;
	ret = 0;
	tmp_str = NULL;
	sep = NULL;
	submod = NULL;
	sep_head = NULL;
	submod_head = NULL;
	split_submod_and_sep(&str, &submod_head, &sep_head);
	if (!sep_head)
	{
		while (str[i])
		{
			if (str[i] == '\"' || str[i] == '\'')
			{
				k = go_to_closing_char(&str[i]);
				i += k;
				j += k;
			}
			if (str[i] != '(' && str[i] != ')')
				j++;
			i++;
		}
		tmp_str = str;
		str = ft_alloc(sizeof(char) * (j + 1));
		i = 0;
		j = 0;
		while (tmp_str[i])
		{
			if (tmp_str[i] == '\"' || tmp_str[i] == '\'')
			{
				k = go_to_closing_char(&tmp_str[i]);
				while(k)
				{
					str[j] = tmp_str[i];
					i++;
					j++;
					k--;
				}
			}
			if (tmp_str[i] && tmp_str[i] != '(' && tmp_str[i] != ')')
			{
				str[j] = tmp_str[i];
				j++;
			}
			i++;
		}
		str[j] = '\0';
		ft_free(tmp_str);
		pipex_ret = ft_pipe(str, envp_head, last_exit_code);
		ft_free(str);
		return (pipex_ret);
	}
	sep = sep_head;
	submod = submod_head;
	while (sep)
	{
		ret = algorithm(ft_strdup((char *)submod->content), envp_head, last_exit_code);
		if (submod_head != submod)
			sep = sep->next;
		submod = submod->next;
		if (sep && *((char *)(sep->content)) == '1' && ret)
		{
			while (sep && *((char *)(sep->content)) == '1')
			{
				submod = submod->next;
				sep = sep->next;
			}
		}
		else if (sep && *((char *)(sep->content)) == '2' && !ret)
		{
			submod = submod->next;
			sep = sep->next;
		}
	}
	ft_free_list(submod_head);
	ft_free_list(sep_head);
	ft_free(str);
	return (ret);
}
