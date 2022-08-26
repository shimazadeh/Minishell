/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnjoin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 23:02:34 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/24 21:57:29 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strnjoin(char const *s1, char const *s2, int size)
{
	int		i;
	int		j;
	int		len;
	char	*str;

	i = -1;
	j = -1;
	len = 0;
	str = NULL;
	if (size < 0)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2);
	if (len > size)
		len = size;
	str = (char *)ft_alloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	while (s1 && s1[++i] && size-- > 0)
		str[i] = s1[i];
	while (s2 && s2[++j] && size-- > 0)
		str[i + j] = s2[j];
	str[i + j] = '\0';
	return (str);
}
