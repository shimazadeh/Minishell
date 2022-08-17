/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 22:59:24 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/15 19:52:22 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_dprint(va_list arg, char c, char **to_print_add)
{
	char					char_var[1];
	char					*str_var;
	unsigned long long int	ulli_var;
	long long int			lli_var;
	char					*char_tmp;
	char					*to_print;

	str_var = NULL;
	ulli_var = 0;
	lli_var = 0;
	char_tmp = NULL;
	if (*to_print_add)
		to_print = *to_print_add;
	else
		to_print = NULL;
	if (c == 'c')
	{
		char_tmp = to_print;
		char_var[0] = (char)va_arg(arg, int);
		to_print = ft_strjoin(char_tmp, char_var);
		free(char_tmp);
	}
	else if (c == 's')
	{
		char_tmp = to_print;
		str_var = va_arg(arg, char *);
		if (str_var)
			to_print = ft_strjoin(char_tmp, str_var);
		else
			to_print = ft_strjoin(char_tmp, "(null)");
	}
	else if (c == 'p')
	{
		char_tmp = to_print;
		to_print = ft_strjoin(char_tmp, "0x");
		free(char_tmp);
		ulli_var = (unsigned long long int)va_arg(arg, void *);
		str_var = ft_ull_itoa_base(ulli_var, "0123456789abcdef");
		char_tmp = to_print;
		to_print = ft_strjoin(char_tmp, str_var);
		free(char_tmp);
	}
	else if (c == 'd' || c == 'i')
	{
		lli_var = (long long int)va_arg(arg, int);
	//	printf("------------------>>>>>>>>> %lld\n", lli_var);
		str_var = ft_ll_itoa_base(lli_var, "0123456789");
	//	printf("------------------>>>>>>>>> %s\n", str_var);
		char_tmp = to_print;
		to_print = ft_strjoin(char_tmp, str_var);
		free(char_tmp);
	}
	else if (c == 'u')
	{
		ulli_var = (unsigned long long int)va_arg(arg, unsigned int);
		str_var = ft_ll_itoa_base(ulli_var, "0123456789");
		char_tmp = to_print;
		to_print = ft_strjoin(char_tmp, str_var);
		free(char_tmp);
	}
	else if (c == 'x')
	{
		ulli_var = (unsigned long long int)va_arg(arg, unsigned int);
		str_var = ft_ll_itoa_base(ulli_var, "0123456789abcdef");
		char_tmp = to_print;
		to_print = ft_strjoin(char_tmp, str_var);
		free(char_tmp);
	}
	else if (c == 'X')
	{
		ulli_var = (unsigned long long int)va_arg(arg, unsigned int);
		str_var = ft_ll_itoa_base(ulli_var, "0123456789ABCDEF");
		char_tmp = to_print;
		to_print = ft_strjoin(char_tmp, str_var);
		free(char_tmp);
	}
	else
	{
		char_tmp = to_print;
		to_print = ft_strjoin(char_tmp, "%");
		free(char_tmp);
	}
	*to_print_add = to_print;
}

int	ft_dprintf(int fd, const char *str, ...)
{
	va_list	arg;
	int		i;
	char	*to_print;
	char	*tmp;
	char	char_add[1];

	i = 0;
	to_print = NULL;
	tmp = NULL;
	va_start(arg, str);
	while (str[i])
	{
		if (str[i] == '%')
		{
			ft_dprint(arg, str[i + 1], &to_print);
			i += 2;
		}
		else
		{
			char_add[0] = str[i];
			tmp = to_print;
		//	printf("I = %d  '%c'  ", i, str[i]);
		//	if (tmp)
		//		printf("TMP = '%s' ", tmp);
		//	printf("C_ADD '%s'\n",char_add);
			to_print = ft_strjoin(tmp, char_add);
			free(tmp);
			i++;
		}
	}
	va_end(arg);
	ft_putstr_fd(to_print, fd);
	return (ft_strlen(to_print));
}
