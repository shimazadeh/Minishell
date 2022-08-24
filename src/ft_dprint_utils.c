/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprint_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 22:13:27 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/24 22:26:12 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_dprint_1(va_list arg, char c, char **to_print_add, \
t_ft_dprint_vars v[1])
{
	char	char_var[1];

	if (*to_print_add)
		v->to_print = *to_print_add;
	if (c == 'c')
	{
		v->char_tmp = v->to_print;
		char_var[0] = (char)va_arg(arg, int);
		v->to_print = ft_strjoin(v->char_tmp, char_var);
		ft_free(v->char_tmp);
	}
	else if (c == 's')
	{
		v->char_tmp = v->to_print;
		v->str_var = va_arg(arg, char *);
		if (v->str_var)
			v->to_print = ft_strjoin(v->char_tmp, v->str_var);
		else
			v->to_print = ft_strjoin(v->char_tmp, "(null)");
	}
	else
		ft_dprint_2(arg, c, v);
}

void	ft_dprint_2(va_list arg, char c, t_ft_dprint_vars v[1])
{
	if (c == 'p')
	{
		v->char_tmp = v->to_print;
		v->to_print = ft_strjoin(v->char_tmp, "0x");
		ft_free(v->char_tmp);
		v->ulli_var = (unsigned long long int)va_arg(arg, void *);
		v->str_var = ft_ull_itoa_base(v->ulli_var, "0123456789abcdef");
		v->char_tmp = v->to_print;
		v->to_print = ft_strjoin(v->char_tmp, v->str_var);
		ft_free(v->char_tmp);
	}
	else if (c == 'd' || c == 'i')
	{
		v->lli_var = (long long int)va_arg(arg, int);
		v->str_var = ft_ll_itoa_base(v->lli_var, "0123456789");
		v->char_tmp = v->to_print;
		v->to_print = ft_strjoin(v->char_tmp, v->str_var);
		ft_free(v->char_tmp);
	}
	else
		ft_dprint_3(arg, c, v);
}

void	ft_dprint_3(va_list arg, char c, t_ft_dprint_vars v[1])
{
	if (c == 'u')
	{
		v->ulli_var = (unsigned long long int)va_arg(arg, unsigned int);
		v->str_var = ft_ll_itoa_base(v->ulli_var, "0123456789");
		v->char_tmp = v->to_print;
		v->to_print = ft_strjoin(v->char_tmp, v->str_var);
		ft_free(v->char_tmp);
	}
	else if (c == 'x')
	{
		v->ulli_var = (unsigned long long int)va_arg(arg, unsigned int);
		v->str_var = ft_ll_itoa_base(v->ulli_var, "0123456789abcdef");
		v->char_tmp = v->to_print;
		v->to_print = ft_strjoin(v->char_tmp, v->str_var);
		ft_free(v->char_tmp);
	}
	else
		ft_dprint_4(arg, c, v);
}

void	ft_dprint_4(va_list arg, char c, t_ft_dprint_vars v[1])
{
	if (c == 'X')
	{
		v->ulli_var = (unsigned long long int)va_arg(arg, unsigned int);
		v->str_var = ft_ll_itoa_base(v->ulli_var, "0123456789ABCDEF");
		v->char_tmp = v->to_print;
		v->to_print = ft_strjoin(v->char_tmp, v->str_var);
		ft_free(v->char_tmp);
	}
	else
	{
		v->char_tmp = v->to_print;
		v->to_print = ft_strjoin(v->char_tmp, "%");
		ft_free(v->char_tmp);
	}
}
