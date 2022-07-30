/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shabibol <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 04:26:21 by shabibol          #+#    #+#             */
/*   Updated: 2022/03/04 21:17:02 by shabibol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		j;
	int		k;
	char	*dst;

	if (s1 == NULL || s2 == NULL)
		return (0);
	dst = (char *) malloc (sizeof (char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (dst == NULL)
		return (0);
	j = 0;
	k = 0;
	while (s1[j] != '\0')
	{
		dst[j] = s1[j];
		j++;
	}
	while (s2[k] != '\0')
		dst[j++] = s2[k++];
	dst[j] = '\0';
	return (dst);
}
