/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_3.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shabibol <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 00:29:19 by shabibol          #+#    #+#             */
/*   Updated: 2022/10/04 00:29:22 by shabibol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_last_ec(char *str, int *i, int last_ec)
{
	char	*var_exp;
	char	*new_str;

	var_exp = ft_itoa(last_ec);
	new_str = create_new_str(str, var_exp, i, *i + 2);
	ft_free(str);
	return (new_str);
}
