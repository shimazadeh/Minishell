/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_util_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shabibol <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 20:29:14 by shabibol          #+#    #+#             */
/*   Updated: 2022/08/25 20:29:16 by shabibol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	number_of_here_doc(char *str)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\"' || str[i] == '\'')
			i += go_to_closing_char((char *)&str[i]) + 1;
		else if (str[i] == '<' && str[i + 1] == '<')
		{
			count++;
			i++;
		}
		else
			i++;
	}
	return (count);
}

int	pass_the_next_word(char *str)
{
	int	i;

	i = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (str[i] && (str[i] == '\'' || str[i] == '\"'))
		return (i + go_to_closing_char(&str[i]) + 1);
	while (str[i] && str[i] != ' ' && str[i] != '\t')
		i++;
	if (str[i])
		return (i);
	return (0);
}
