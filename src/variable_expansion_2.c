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

char	*expand_variable(char *str, int *i, t_list **envp_head, int flag)
{
	char	*var_exp;
	char	*var_name;
	int		end;
	char	*new_str;

	end = *i + 1;
	var_exp = NULL;
	var_name = NULL;
	if (end_char(str[end], " +=/.,~:%^$"))
		return ((*i)++, str);
	else if (str[end] >= '0' && str[end] <= '9')
		end++;
	else if (!end_char(str[end], " ./~@!$\n^%#:+-="))
	{
		while (str[end] && !end_char(str[end], " ./~!$'\"\n^%#:+@-="))
			end++;
		var_name = ft_strdup_range(str, *i + 1, end);
		find_env_var(var_name, envp_head, &var_exp);
		ft_free(var_name);
		if (flag == 0)
			add_double_quotes(&var_exp);
	}
	new_str = create_new_str(str, var_exp, i, end);
	ft_free(str);
	return (new_str);
}

int	add_double_quotes(char	**str_add)
{
	int			i;
	char		**result;
	char		**tab;
	char		*str;

	if (!str_add || !*str_add)
		return (0);
	str = *str_add;
	tab = ft_split(str, ' ');
	result = ft_alloc(sizeof(char *) * (ft_strlen_tab(tab) + 1));
	i = 0;
	while (tab[i])
	{
		result[i] = ft_alloc(sizeof(char) * (ft_strlen(tab[i]) + 3 + 1));
		ft_strcpy_wquotes(result[i], tab[i], ft_strlen_tab(tab) - 1, i);
		i++;
	}
	result[i] = '\0';
	ft_free_tab(tab);
	ft_free(str);
	*str_add = ft_jointab(result);
	ft_free(result);
	return (0);
}

int	ft_strcpy_wquotes(char *dest, char *str, int limit, int i)
{
	size_t	k;
	int		j;

	k = 0;
	j = 0;
	dest[j++] = '\"';
	while (k < ft_strlen(str))
	{
		dest[j] = str[k];
		j++;
		k++;
	}
	dest[j++] = '\"';
	if (i < limit)
		dest[j++] = ' ';
	dest[j] = '\0';
	return (0);
}
