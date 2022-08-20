/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 17:21:24 by shabibol          #+#    #+#             */
/*   Updated: 2022/08/20 03:21:07 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *create_new_str(char *str, char *to_add, int to_break, int to_continue)
{
	char *tmp;
	char *tmp2;
	char *new_str;

	if (to_continue <= to_break)
		return (str);
	tmp = ft_strdup_range(str, 0, to_break);
	tmp2 = ft_strjoin(tmp, to_add);
	ft_free(tmp);
	tmp = ft_strdup_range(str, to_continue, ft_strlen(str));
	new_str = ft_strjoin(tmp, tmp2);
	ft_free(tmp);
	ft_free(tmp2);
	return (new_str);
}

int	variable_expansion(char **str_add, t_list **envp_head, int last_exit_code)
{
	int i;
	int j;
	int k;
	char *var_name;
	char *var_exp;
	char *str;

	i = 0;
	j = 0;
	str = *str_add;
	while(str[i])
	{
		if (str[i] && (str[i] == '\"' || str[i] == '\''))
		{
			k = go_to_closing_char(&str[i]);
			if (k)
			{
				str[i] = ' ';
				i += k;
				str[i] = ' ';
			}
		}
		else if (str[i] && str[i] == '$' && str[i + 1] && str[i + 1] != ' ')
		{
			i++;
			j = i;
			if (str[i] == '?')
			{
				var_exp = ft_itoa(last_exit_code);
				i++;
			}
			else if (str[i])
			{
				while(str[i] && str[i] != ' ')
					i++;
				var_name = ft_strdup_range(str, j, i);
				find_env_var(var_name, envp_head, &var_exp);
				ft_free(var_name);
			}
			str = create_new_str(str, var_exp, j - 1, i);
			i = 0;
		}
		else
			i++;
	}
	*str_add = str;
	return (0);
}

