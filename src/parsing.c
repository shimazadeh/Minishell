/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/28 12:33:23 by shabibol          #+#    #+#             */
/*   Updated: 2022/08/22 20:08:16 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	number_of_delim(char *str, char delim, int flag)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (flag == 0 && str[i] == delim && str[i + 1] != delim)
			count++;
		else if (flag == 0 && str[i] == delim && str[i + 1] == delim)
			i++;
		else if (flag == 1 && str[i] == delim && str[i + 1] == delim)
		{
			count++;
			i++;
		}
		else if (flag == 1 && str[i] == delim && str[i + 1] != delim)
			count++;
		i++;
	}
	return (count);
}

int	save_the_next_word(char **str_add, int i, char **dest, int to_clean)
{
	int		start;
	char	end;
	char	*str;

	str = *str_add;
	end = ' ';
	to_clean--;
	while (str[i] && str[i] == ' ')
		i++;
	if (str[i] == '\'' || str[i] == '\"')
	{
		end = str[i];
		i++;
	}
	start = i;
	while (str[i] && str[i] != end && str[i] != '<' && str[i] != '>')
		i++;
	*dest = ft_strdup_range(str, start, i);
	if (str[i] == end)
		str[i] = ' ';
	while (++to_clean < i)
		str[to_clean] = ' ';
	*str_add = str;
	return (i);
}

char	**parse_infiles(char **str_add)
{
	int			dest_size;
	char		**dest;
	char		*str;
	int			i;
	int			k;

	i = 0;
	k = -1;
	str = *str_add;
	dest_size = number_of_delim(str, '<', 0);
	if (!dest_size)
		return (NULL);
	dest = ft_alloc(sizeof(char *) * (dest_size + 1));
	while (str[i])
	{
		if (str[i] == '\"' || str[i] == '\'')
			i = i + go_to_closing_char(&str[i]) + 1;
		else if (str[i] && str[i + 1] && str[i] == '<' && str[i + 1] != '<')
			i = save_the_next_word(&str, i + 1, &dest[++k], i);
		else
			i++;
	}
	dest[++k] = '\0';
	*str_add = str;
	return (dest);
}

char	**parse_outfiles(char **str_add, t_struct *head)
{
	char		**dest;
	char		*str;
	int			i;
	int			k;

	i = 0;
	str = *str_add;
	k = -1;
	if (!number_of_delim(str, '>', 1))
		return (NULL);
	dest = ft_alloc(sizeof(char *) * (number_of_delim(str, '>', 1) + 1));
	head->outfile_modes = ft_alloc(sizeof(int) * number_of_delim(str, '>', 1));
	while (str[i])
	{
		if (str[i] == '\"' || str[i] == '\'')
			i = i + go_to_closing_char(&str[i]) + 1;
		else if (str[i] && str[i] == '>' && str[i + 1] && str[i + 1] != '>')
		{
			i = save_the_next_word(&str, i + 1, &dest[++k], i);
			head->outfile_modes[k] = 1;//o_trunc
		}
		else if (str[i] && str[i] == '>' && str[i + 1] && str[i + 1] == '>')
		{
			i = save_the_next_word(&str, i + 2, &dest[++k], i);
			head->outfile_modes[k] = 2;//append mode
		}
		else
			i++;
	}
	dest[++k] = '\0';
	*str_add = str;
	return (dest);
}

int	set_infiles_outfiles_cmds(t_struct **elements)
{
	t_struct	*copy;

	copy = *elements;
	while (copy)
	{
		// dprintf(2, "str is: %s\n", copy->str);
		copy->infiles = parse_infiles(&copy->str);
		// dprintf(2, "str in parsing before split/infile: %s\n", copy->str);
		copy->outfiles = parse_outfiles(&copy->str, copy);
		// dprintf(2, "str in parsing before split/outfile: %s\n", copy->str);
		// dprintf(2, "str is : --%s--\n", copy->str);
		copy->cmd = ft_split_custom(copy->str, ' ', 1);
		// print_tab(copy->cmd);
		handle_wildcards(&copy->cmd);
		copy = copy->next;
	}
	return (0);
}
