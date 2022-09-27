/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 17:21:24 by shabibol          #+#    #+#             */
/*   Updated: 2022/09/23 16:30:21 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	end_char(char c, char *end)
{
	int	i;

	i = 0;
	if (!c)
		return (1);
	while (end[i])
	{
		if (c == end[i])
			return (1);
		i++;
	}
	return (0);
}

char	*create_new_str(char *str, char *to_add, int *to_break, int to_continue)
{
	char	*tmp;
	char	*tmp2;
	char	*new_str;

	if (to_continue <= *to_break)
		return (str);
	tmp = ft_strdup_range(str, 0, *to_break);
	tmp2 = ft_strjoin(tmp, to_add);
	ft_free(tmp);
	tmp = ft_strdup_range(str, to_continue, ft_strlen(str));
	new_str = ft_strjoin(tmp2, tmp);
	ft_free(tmp);
	ft_free(tmp2);
	*to_break = *to_break + ft_strlen(to_add);
	return (new_str);
}

char	*expand_variable(char *str, int *i, int last_ec, t_list **envp_head)
{
	char	*var_exp;
	char	*var_name;
	int		end;
	char	*new_str;

	end = *i + 1;
	var_name = NULL;
	var_exp = NULL;
	if (str[end] == '?')
	{
		var_exp = ft_itoa(last_ec);
		end++;
	}
	else if (str[end] != '\'' && str[end] != '\"')
	{
		while (str[end] && !end_char(str[end], " $'\"\n^%#:+@-"))
			end++;
		var_name = ft_strdup_range(str, *i + 1, end);
		find_env_var(var_name, envp_head, &var_exp);
		ft_free(var_name);
	}
	new_str = create_new_str(str, var_exp, i, end);
	ft_free(str);
	return (new_str);
}

char	*variable_expansion(char *str, t_list **envp_head, int last_ec)
{
	int		flag;
	int		i;
	char	*delim;

	flag = -1;
	i = 0;
	delim = "$ \t^%#:+@-";
	while (str[i])
	{
		if (str[i] == '<' && str[i + 1] == '<')
			i = i + 2 + pass_the_next_word(&str[i + 2]);
		else if (str[i] == '\'' && i > flag)
			i += go_to_closing_char(&str[i]) + 1;
		else if (str[i] == '\"' && i > flag)
		{
			flag = i + go_to_closing_char(&str[i]);
			i++;
		}
		else if (str[i] == '$' && !end_char(str[i + 1], delim) && i + 1 != flag)
			str = expand_variable(str, &i, last_ec, envp_head);
		else
			i++;
	}
	return (str);
}

char	*variable_expansion_hd(char **str_add, t_list **envp_head, int last_ec)
{
	int		i;
	char	*str;

	i = 0;
	str = *str_add;
	while (str[i])
	{
		if (str[i] == '$' && (str[i + 1] == '\'' || str[i + 1] == '\"'))
			i += go_to_closing_char(&str[i]) + 1;
		else if (str[i] == '$' && !end_char(str[i + 1], "$ \t^%#:+@-\n"))
			str = expand_variable(str, &i, last_ec, envp_head);
		else
			i++;
	}
	return (ft_free(str_add), str);
}
