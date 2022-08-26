/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 22:59:24 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/26 22:36:35 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_dprint_init_vars(char **to_print_add, t_ft_dprint_vars v[1])
{
	v->str_var = NULL;
	v->ulli_var = 0;
	v->lli_var = 0;
	v->char_tmp = NULL;
	v->to_print = NULL;
	v->to_print = *to_print_add;
}

int	ft_dprintf(int fd, const char *str, ...)
{
	va_list	arg;
	int		i;
	char	*to_print;
	char	*tmp;
	char	char_add[1];

	i = -1;
	to_print = NULL;
	tmp = NULL;
	va_start(arg, str);
	while (str[++i])
	{
		if (str[i] == '%')
			ft_dprint(arg, str[++i], &to_print);
		else
		{
			char_add[0] = str[i];
			tmp = to_print;
			to_print = ft_strjoin(tmp, char_add);
			ft_free(tmp);
		}
	}
	va_end(arg);
	ft_putstr_fd(to_print, fd);
	return (ft_strlen(to_print));
}

void	ft_dprint(va_list arg, char c, char **to_print_add)
{
	t_ft_dprint_vars	v[1];

	ft_dprint_init_vars(to_print_add, v);
	ft_dprint_1(arg, c, to_print_add, v);
	*to_print_add = v->to_print;
}
