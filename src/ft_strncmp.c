/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 12:12:12 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/11 18:29:51 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strncmp(const char *s1, const char *s2, int n)
{
	int	i;

	i = 0;
	if (!s1)
		return (0);
	if (n == 0)
		return (0);
	while ((unsigned char)s1[i] == (unsigned char)s2[i] && (unsigned char)s1[i]
		&& (i < n - 1 || n == -1))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
