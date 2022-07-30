/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shabibol <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 04:32:28 by shabibol          #+#    #+#             */
/*   Updated: 2022/03/05 07:02:22 by shabibol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	n;
	size_t	m;

	i = 0;
	n = ft_strlen(dst);
	m = ft_strlen(src);
	if (dstsize == 0)
		return (m);
	while (src[i] != '\0' && (i + n) < (dstsize - 1))
	{
		dst[i + n] = src[i];
		i++;
	}
	if (dstsize > n)
	{
		dst[i + n] = '\0';
		return (n + m);
	}
	return (dstsize + m);
}
