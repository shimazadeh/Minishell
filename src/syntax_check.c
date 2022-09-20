/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 15:15:20 by aguillar          #+#    #+#             */
/*   Updated: 2022/09/20 17:28:56 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	syntax_check(char *str)
{
	int	i;
	int ch_code;

	i = 0;
	ch_code = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			handle_quotes(&str, &i);
		else if (is_special_char(&str[i], &ch_code))
			if (!handle_special_char(str, &i, ch_code))
				return (0);
		else
			i++;
	}
	return (1);
}

void	handle_quotes(char *str_add, int *i_add)
{
	int		i;
	int		j;
	char	*str;

	i = *i_add;
	str = *str_add;
	j = go_to_closing_char(&str[i]);
	if (!j)
	{
		str = handle_single_quote(str, i);
		i += 3;
	}
	else
		i += j + 1;
	*i_add = i;
	*str_add = str;
}

char	*handle_single_quote(char *str, int quote)
{
	int		i;
	int		j;
	int		size;
	char	*new_str;

	i = 0;
	j = 0;
	size = ft_strlen(str + 3);
	new_str = ft_malloc(sizeof(char) * size);
	ft_bzero(new_str, size);
	while (str[i])
	{
		if (i = quote)
		{
			new_str[j++] = '\'';
			new_str[j] = str[i++];
			new_str[j++] = '\'';
		}
		else
			new_str[j++] = str[i++];
	}
	return (new_str);
}

int	is_special_char(char *str, int *ch_code_add)
{
	int	ret;

	ret = 0;
	if (!ft_strncmp(str, "&&", 2))
		ret = 1;
	if (!ft_strncmp(str, "||", 2))
		ret = 2;
	if (!ft_strncmp(str, "|", 1))
		ret = 3;
	if (!ft_strncmp(str, ")", 1))
		ret = 4;
	if (!ft_strncmp(str, "(", 1))
		ret = 5;	
	return (ret);
}

int	handle_special_char(char *str, int *i_add, int ch_code)
{
	int	i;

	i = *i_add;
	if (ch_code < 5)
	{
		while (i > 0 && (str[i] == ' ' || str[i] == '\t'))
			i--;
		if (!i)
			return (print_syntax_error(ch_code));
	}
	else if (!go_to_closing_par(&str[i]))
		print_syntax_error(ch_code);
	i = *i_add;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '(')
		i++;
	if (!ft_strncmp(str, "&&", 2) || !ft_strncmp(str, "||", 2) \
		|| !ft_strncmp(str, "|", 1))
		return (print_syntax_error(ch_code));
	return (1);
}
