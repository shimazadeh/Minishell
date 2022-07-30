/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shabibol <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 04:32:12 by shabibol          #+#    #+#             */
/*   Updated: 2022/03/04 22:41:18 by shabibol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *lst, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	i = 0;
	if (lst != NULL && f != NULL)
	{
		while (lst[i] != '\0')
		{
			(*f)(i, &lst[i]);
			i++;
		}
	}
}
