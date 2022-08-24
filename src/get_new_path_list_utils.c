/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_new_path_list_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 23:51:05 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/25 00:12:28 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	next_char_index(char *str, char c)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (0);
	while (str[i] && str[i] != c)
		i++;
	if (!str[i])
		return (i + 1);
	return (i);
}

int	compatible_name(char *file, char *wc)
{
	int						i;
	int						j;
	int						k;
	t_compatible_name_vars	v[1];

	i = 0;
	j = 1;
	k = 0;
	if (!file || !wc)
		return (0);
	v->file = file;
	v->wc = wc;
	if (!compatible_name_init(&i, &j, &k, v))
		return (0);
	while (file[i])
		compatible_name_loop(&i, &j, &k, v);
	if (!v->wc[j])
		return (1);
	return (0);
}

int	compatible_name_init(int *i_add, int *j_add, int *k_add, \
t_compatible_name_vars v[1])
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 1;
	k = 0;
	if (v->file[0] == '.' && v->wc[0] != '.')
		return (0);
	if (v->wc[0] != '*')
	{
		k = next_char_index(v->wc, '*');
		if (ft_strncmp(v->wc, v->file, k))
			return (0);
		else
		{
			i = k;
			j = k + 1;
		}
	}
	*i_add = i;
	*j_add = j;
	*k_add = k;
	return (1);
}

void	compatible_name_loop(int *i_add, int *j_add, int *k_add, \
t_compatible_name_vars v[1])
{
	int	i;
	int	j;
	int	k;

	i = *i_add;
	j = *j_add;
	k = 0;
	k = next_char_index(&(v->wc)[j], '*');
	if (v->wc[j] && !ft_strncmp(&(v->file)[i], &(v->wc)[j], k))
	{
		j += k - 1;
		i += k - 1;
		if (v->wc[j])
			j += 2;
		if (v->file[i])
			i++;
	}
	else
		i++;
	*i_add = i;
	*j_add = j;
	*k_add = k;
}
