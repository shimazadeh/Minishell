/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 04:26:14 by shabibol          #+#    #+#             */
/*   Updated: 2022/08/18 14:42:07 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strdup(const char *src)
{
	int		size;
	char	*s2;
	int		i;

	i = 0;
	size = 0;
	if (!src)
		return (NULL);
	while (src[size] != '\0')
		size++;
	s2 = ft_alloc(sizeof(char) * (size + 1));
	if (s2 == NULL)
		return (NULL);
	while (src[i] != '\0')
	{
		s2[i] = src[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}
