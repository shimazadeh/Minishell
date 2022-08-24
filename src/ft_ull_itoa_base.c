/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ull_itoa_base.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 22:02:31 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/24 22:28:50 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_ull_nb_len(unsigned long long int nb, int base_len)
{
	int			i;

	i = 0;
	if (nb == 0)
		return (1);
	while (nb > 0)
	{
		nb = nb / (unsigned long long)base_len;
		i++;
	}
	return (i);
}

static char	*ft_ull_nbr_to_string(unsigned long long int nb, \
char *str, int nb_len, char *base)
{
	int						base_len;

	base_len = ft_strlen(base);
	while (nb_len > 0)
	{
		str[nb_len - 1] = base[nb % (unsigned long long int)base_len];
		nb = nb / (unsigned long long int)base_len;
		nb_len--;
	}
	return (str);
}

char	*ft_ull_itoa_base(unsigned long long nb, char *base)
{
	int			nb_len;
	int			base_len;
	char		*str;

	base_len = ft_strlen(base);
	nb_len = ft_ull_nb_len(nb, base_len);
	str = (char *)ft_alloc(sizeof(char) * (nb_len + 1));
	if (!str)
		return (NULL);
	if (nb == 0)
		str[0] = base[0];
	else
		str = ft_ull_nbr_to_string(nb, str, nb_len, base);
	str[nb_len] = '\0';
	return (str);
}
