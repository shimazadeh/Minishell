/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithm.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 14:27:04 by aguillar          #+#    #+#             */
/*   Updated: 2022/09/27 15:14:33 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	algorithm_init_vars(t_aglorithm_vars v[1])
{
	v->pipex_ret = 0;
	v->ret = 0;
	v->tmp_str = NULL;
	v->sep = NULL;
	v->submod = NULL;
	v->sep_head = NULL;
	v->submod_head = NULL;
}

int	algorithm(char *str, t_list **envp_head, int last_exit_code)
{
	int					ret;
	t_aglorithm_vars	v[1];

	ret = 0;
	algorithm_init_vars(v);
	if (g_var->sig_flag == 1)
		last_exit_code = 130;
	split_submod_and_sep_1(&str, &(v->submod_head), &(v->sep_head));
	if (!v->sep_head)
		ret = algorithm_exit_condition_1(str, envp_head, last_exit_code, v);
	else
		ret = algorithm_recursive(envp_head, last_exit_code, v);
	ft_free(str);
	ft_free(v->tmp_str);
	ft_free_list(v->sep_head);
	ft_free_list(v->submod_head);
	return (ret);
}

int	algorithm_exit_condition_1(char *str, t_list **envp_head, \
int last_exit_code, t_aglorithm_vars v[1])
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	k = 0;
	while (str[i])
	{
		if (str[i] == '\"' || str[i] == '\'')
		{
			k = go_to_closing_char(&str[i]);
			i += k;
			j += k;
		}
		if (str[i] && str[i] != '(' && str[i] != ')')
			j++;
		i++;
	}
	v->tmp_str = str;
	str = ft_alloc(sizeof(char) * (j + 1));
	return (algorithm_exit_condition_2(str, envp_head, last_exit_code, v));
}

int	algorithm_exit_condition_2(char *str, t_list **envp_head,
int last_exit_code, t_aglorithm_vars v[1])
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	k = 0;
	while (v->tmp_str[i])
	{
		if (v->tmp_str[i] == '\"' || v->tmp_str[i] == '\'')
		{
			k = go_to_closing_char(&v->tmp_str[i]);
			while (k--)
				str[j++] = v->tmp_str[i++];
		}
		if (v->tmp_str[i] && v->tmp_str[i] != '(' && v->tmp_str[i] != ')')
			str[j++] = v->tmp_str[i];
		i++;
	}
	str[j] = '\0';
	g_var->sig_flag = 0;
	v->pipex_ret = ft_pipe(str, envp_head, last_exit_code);
	return (v->pipex_ret);
}

int	algorithm_recursive(t_list **envp_head, int last_exit_code,
t_aglorithm_vars v[1])
{
	v->sep = v->sep_head;
	v->submod = v->submod_head;
	while (v->sep)
	{
		v->ret = algorithm(ft_strdup((char *)v->submod->content),
				envp_head, last_exit_code);
		if (v->submod_head != v->submod)
			v->sep = v->sep->next;
		v->submod = v->submod->next;
		if (v->sep && *((char *)(v->sep->content)) == '1' && v->ret)
		{
			while (v->sep && *((char *)(v->sep->content)) == '1')
			{
				v->submod = v->submod->next;
				v->sep = v->sep->next;
			}
		}
		else if (v->sep && *((char *)(v->sep->content)) == '2' && !v->ret)
		{
			v->submod = v->submod->next;
			v->sep = v->sep->next;
		}
	}
	return (v->ret);
}
