/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shabibol <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 18:55:21 by shabibol          #+#    #+#             */
/*   Updated: 2022/10/01 18:55:23 by shabibol         ###   ########.fr       */
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
	else if (str[end] >= '0' && str[end] <= '9')
		end++;
	else if (!end_char(str[end], " ./~@!$\n^%#:+-="))
	{
		while (str[end] && !end_char(str[end], " ./~!$'\"\n^%#:+@-="))
			end++;
		var_name = ft_strdup_range(str, *i + 1, end);
		find_env_var(var_name, envp_head, &var_exp);
		ft_free(var_name);
	}
	new_str = create_new_str(str, var_exp, i, end);
	return (*i = 0, ft_free(str), new_str);
}
