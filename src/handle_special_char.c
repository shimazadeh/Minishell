/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_special_char.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 21:51:58 by aguillar          #+#    #+#             */
/*   Updated: 2022/09/22 23:05:53 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_special_char(char *str, int *i_add, int ch_code, char *par_tab)
{
	int	i;

	i = *i_add;
	if (ch_code < 5 && !handle_special_char_loop_check_prev(i, ch_code, str))
		return (0);
	if (ch_code == 5 && !go_to_closing_par(&str[i], &par_tab[i]))
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
	if (!handle_special_char_loop(i, ch_code, str))
		return (0);
	if (ch_code <= 2)
		(*i_add)++;
	return (1);
}

int	handle_special_char_loop(int i, int ch_code, char *str)
{
	while (i > 0 && (str[i] == ' ' || str[i] == '\t' \
	|| (ch_code == 5 && str[i] == '(') || (ch_code == 4 && str[i] == ')')))
		i--;
	if (i >= 0 && ((i && ch_code != 5 && !ft_strncmp(&str[i - 1], "&&", 2)) \
		|| (i && ch_code != 5 && !ft_strncmp(&str[i - 1], "||", 2)) \
		|| (ch_code != 5 && !ft_strncmp(&str[i], "|", 1)) \
		|| (ch_code == 5 && str[i] == ')')
			|| (ch_code != 5 && str[i] == '(')))
		return (print_syntax_error(ch_code));
	return (1);
}

int	handle_special_char_loop_check_prev(int i, int ch_code, char *str)
{
	if (ch_code < 5)
	{
		if (i)
			i--;
		while (i > 0 && (str[i] == ' ' || str[i] == '\t'))
			i--;
		if (!i)
			return (print_syntax_error(ch_code));
	}
	return (1);
}
