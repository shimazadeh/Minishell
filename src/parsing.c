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
	if (!str || !delim)
		return (0);
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

void	parse_infiles(char **str_add, t_struct *head)
{
	char		*str;
	int			i;
	int			k;

	i = 0;
	k = -1;
	str = *str_add;
	while (str[i])
	{
		if (str[i] == '\"' || str[i] == '\'')
			i = i + go_to_closing_char(&str[i]) + 1;
		else if (str[i] && str[i + 1] && str[i] == '<' && str[i + 1] != '<')
		{
			i = save_the_next_word(&str, i + 1, &head->infiles[++k], i);
			head->infile_modes[k] = 0;
		}
		else
			i++;
	}
	head->infiles[++k] = '\0';
	*str_add = str;
}

void	parse_outfiles(char **str_add, t_struct *head)
{
	char		*str;
	int			i;
	int			k;

	i = -1;
	k = -1;
	str = *str_add;
	while (str[++i])
	{
		if (str[i] == '\"' || str[i] == '\'')
			i = i + go_to_closing_char(&str[i]);
		else if (str[i] && str[i] == '>' && str[i + 1] && str[i + 1] != '>')
		{
			i = save_the_next_word(&str, i + 1, &head->outfiles[++k], i) - 1;
			head->outfile_modes[k] = 1;
		}
		else if (str[i] && str[i] == '>' && str[i + 1] && str[i + 1] == '>')
		{
			i = save_the_next_word(&str, i + 2, &head->outfiles[++k], i) - 1;
			head->outfile_modes[k] = 2;
		}
	}
	head->outfiles[++k] = '\0';
	*str_add = str;
}

void	set_infiles_outfiles_cmds(t_struct **elements)
{
	t_struct	*copy;
	int			infile_size;
	int			outfile_size;

	copy = *elements;
	while (copy)
	{
		infile_size = number_of_delim(copy->str, '<', 0);
		if (infile_size)
		{
			copy->infiles = ft_alloc(sizeof(char *) * (infile_size + 1));
			copy->infile_modes = ft_alloc(sizeof(int) * infile_size);
			parse_infiles(&copy->str, copy);
		}
		outfile_size = number_of_delim(copy->str, '>', 1);
		if (outfile_size)
		{
			copy->outfiles = ft_alloc(sizeof(char *) * (outfile_size + 1));
			copy->outfile_modes = ft_alloc(sizeof(int) * outfile_size);
			parse_outfiles(&copy->str, copy);
		}
		copy->cmd = ft_split_custom(copy->str, ' ', 1);
		handle_wildcards(&copy->cmd);
		copy = copy->next;
	}
}
