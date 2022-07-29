/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 14:44:39 by aguillar          #+#    #+#             */
/*   Updated: 2022/05/16 16:00:46 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*ft_strndup(char *s1, int j)
{
	int		i;
	char	*dup;

	i = 0;
	while ((char)s1[i] && (j - i))
		i++;
	dup = malloc(sizeof(char) * (i + 1));
	if (!dup)
		return (0);
	i = 0;
	while ((char)s1[i] && (j - i))
	{
		dup[i] = (char)s1[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}
