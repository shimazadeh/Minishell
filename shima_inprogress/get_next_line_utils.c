/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shabibol <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/13 18:36:30 by shabibol          #+#    #+#             */
/*   Updated: 2022/04/13 18:36:55 by shabibol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_strlen(const char *s1)
{
	int	i;

	i = 0;
	if (!s1)
		return (0);
	while (s1[i] != '\0')
		i++;
	return (i);
}

char	*ft_strndup(char *src, int size)
{
	int		i;
	char	*dest;

	i = 0;
	if (!src || !*src)
		return (NULL);
	dest = (char *)malloc(sizeof(char) * size + 1);
	if (!dest)
		return (NULL);
	while (src[i] && i < size)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strjoin(const char *s1, const char *s2)
{
	int		i;
	int		j;
	char	*dst;

	i = 0;
	j = 0;
	dst = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2)) + 1);
	if (!dst)
		return (NULL);
	while (s1 && s1[i] != '\0')
	{
		dst[i] = s1[i];
		i++;
	}
	while (s2 && s2[j] != '\0')
		dst[i++] = s2[j++];
	dst[i] = '\0';
	return (dst);
}

int	ft_gnl_strchr(char *s1)
{
	if (!s1 || s1 == NULL)
		return (0);
	while (*s1 && *s1 != '\n')
		s1++;
	if (*s1 == '\n')
		return (1);
	return (0);
}
