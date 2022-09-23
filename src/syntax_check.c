/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 15:15:20 by aguillar          #+#    #+#             */
/*   Updated: 2022/09/23 16:35:37 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	syntax_check(char **str_add)
{
	int		i;
	int		ch_code;
	char	*par_tab;
	int		size;
	char	*str;

	str = *str_add;
	size = sizeof(char) * (ft_strlen(str) + 1);
	i = 0;
	ch_code = 0;
	par_tab = ft_alloc(size);
	ft_bzero(par_tab, size);
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			handle_quotes(&str, &i);
		else if (is_special_char(&str[i], &ch_code) && \
			!handle_special_char(str, &i, ch_code, par_tab))
			return (0);
		else
			i++;
	}
	*str_add = str;
	return (1);
}

void	handle_quotes(char **str_add, int *i_add)
{
	int		i;
	int		j;
	char	*str;

	i = *i_add;
	str = *str_add;
	j = go_to_closing_char(&str[i]);
	if (!j)
	{
		str = handle_single_quote(0, 0, str, i);
		i += 3;
	}
	else
		i += j + 1;
	*i_add = i;
	*str_add = str;
}

char	*handle_single_quote(int i, int j, char *str, int quote)
{
	int		size;
	char	*new_str;
	char	c;

	if (str[quote] == '\"')
		c = '\'';
	else
		c = '\"';
	size = sizeof(char) * (ft_strlen(str) + 3);
	new_str = ft_alloc(sizeof(char) * size);
	ft_bzero(new_str, size);
	while (str[i])
	{
		if (i == quote)
		{
			new_str[j++] = c;
			new_str[j++] = str[i++];
			new_str[j++] = c;
		}
		else
			new_str[j++] = str[i++];
	}
	ft_free(str);
	return (new_str);
}

int	is_special_char(char *str, int *ch_code_add)
{
	int	ch_code;

	ch_code = 0;
	if (!ft_strncmp(str, "&&", 2))
		ch_code = 1;
	else if (!ft_strncmp(str, "||", 2))
		ch_code = 2;
	else if (!ft_strncmp(str, "|", 1))
		ch_code = 3;
	else if (!ft_strncmp(str, ")", 1))
		ch_code = 4;
	else if (!ft_strncmp(str, "(", 1))
		ch_code = 5;
	*ch_code_add = ch_code;
	return (ch_code);
}

int	print_syntax_error(int ch_code)
{
	char	*token_str;
	char	**token_tab;

	token_str = ft_strdup("&& || | ) (");
	token_tab = ft_split(token_str, ' ');
	ft_dprintf(1, "bash: syntax error near unexpected token `%s'\n", \
		token_tab[ch_code - 1]);
	return (0);
}
