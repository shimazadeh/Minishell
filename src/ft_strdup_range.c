/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup_range.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 22:19:12 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/26 22:20:06 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strdup_range(char *str, int start, int end)
{
	char	*dst;
	int		i;
	int		k;

	if (end == start)
		return (NULL);
	if (end < start)
		return (str);
	k = 0;
	i = start;
	dst = ft_alloc(sizeof(char) * (ft_strlen(str) + 1));
	while (str[i] && i < end)
	{
		dst[k] = str[i];
		k++;
		i++;
	}
	dst[k] = '\0';
	return (dst);
}
