/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ll_itoa_base.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 22:02:31 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/24 21:54:22 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_ll_nb_len(long long int nb, int base_len)
{
	int						i;

	i = 0;
	if (nb < 0)
	{
		i++;
		nb *= -1;
	}
	if (nb == 0)
		return (1);
	while (nb > 0)
	{
		nb = nb / (long long int)base_len;
		i++;
	}
	return (i);
}

static char	*ft_ll_nbr_to_string(long long int nb, \
char *str, int nb_len, char *base)
{
	int						base_len;
	long long int			nb1;

	nb1 = nb;
	base_len = ft_strlen(base);
	if (nb < 0)
	{
		str[0] = '-';
		nb1 = -nb;
	}
	while ((nb_len > 0 && nb >= 0) || nb_len > 1)
	{
		str[nb_len - 1] = base[nb1 % (long long int)base_len];
		nb1 = nb1 / (long long int)base_len;
		nb_len--;
	}
	return (str);
}

char	*ft_ll_itoa_base(long long int nb, char *base)
{
	int			nb_len;
	int			base_len;
	char		*str;

	base_len = ft_strlen(base);
	nb_len = ft_ll_nb_len(nb, base_len);
	str = ft_alloc(sizeof(char) * (nb_len + 1));
	if (!str)
		return (NULL);
	if (nb == 0)
		str[0] = base[0];
	else
		str = ft_ll_nbr_to_string(nb, str, nb_len, base);
	str[nb_len] = '\0';
	return (str);
}
