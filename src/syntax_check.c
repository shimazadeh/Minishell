/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 15:15:20 by aguillar          #+#    #+#             */
/*   Updated: 2022/09/22 20:04:23 by aguillar         ###   ########.fr       */
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
	char	c;

	i = 0;
	j = 0;
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

int	handle_special_char(char *str, int *i_add, int ch_code, char *par_tab)
{
	int	i;

	i = *i_add;
	if (ch_code < 5)
	{
		if (i)
			i--;
		while (i > 0 && (str[i] == ' ' || str[i] == '\t'))
			i--;
		if (!i)
			return (print_syntax_error(ch_code));
	}
	else if (!go_to_closing_par(&str[i], &par_tab[i]))
		return (print_syntax_error(ch_code));
	i = *i_add;
	if (ch_code == 4 && !(par_tab[(i)]))
		return (print_syntax_error(ch_code));
	i++;
	if (ch_code <= 2)
		i++;
	if (ch_code != 4 && str_is_only_spaces(&str[i]))
		return (print_syntax_error(ch_code));
	i = (*i_add) - 1;
	while (i > 0 && (str[i] == ' ' || str[i] == '\t' \
	|| (ch_code == 5 && str[i] == '(') || (ch_code == 4 && str[i] == ')')))
		i--;
	if (i >= 0 && ((i && ch_code != 5 && !ft_strncmp(&str[i - 1], "&&", 2)) \
		|| (i && ch_code != 5 && !ft_strncmp(&str[i - 1], "||", 2)) \
		|| (ch_code != 5 && !ft_strncmp(&str[i], "|", 1)) \
		|| (ch_code == 5 && str[i] == ')')
		|| (ch_code != 5 && str[i] == '(')))
		return (print_syntax_error(ch_code));
	if (ch_code <= 2)
		(*i_add)++;
	return (1);
}

int	print_syntax_error(int ch_code)
{
	char	*token_str;
	char	**token_tab;

	token_str = ft_strdup("&& || | ) (");
	token_tab = ft_split(token_str, ' ');
	dprintf(1, "bash: syntax error near unexpected token `%s'\n", \
		token_tab[ch_code - 1]);
	return (0);
}
