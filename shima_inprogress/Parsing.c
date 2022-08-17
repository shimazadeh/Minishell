/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shabibol <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/28 12:33:23 by shabibol          #+#    #+#             */
/*   Updated: 2022/07/28 12:33:25 by shabibol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char *ft_strdup_range(char *str, int start, int end)
{
	char *dst;
	int i;
	int k;

	k = 0;
	i = start;
	dst = malloc(sizeof(char) *ft_strlen(str) + 1);
	while (str[i] && i < end)
	{
		dst[k] = str[i];
		k++;
		i++;
	}
	dst[k] = '\0';
	return (dst);
}

int number_of_delim(char *str, char delim, int flag)//if flag == 1 it also counts >> as one
{
	int i;
	int count;

	count = 0;
	i = 0;
	while(str[i])
	{
		if (flag == 0 && str[i] == delim && str[i + 1] != delim)
			count++;
		else if (flag == 0 && str[i] == delim && str[i + 1] == delim)//skip cuz we aint care
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

int save_the_next_word(char **str, int i, char **dest, int to_clean)
{
	int start;

	while((*str)[i] && (*str)[i] == ' ')
		i++;
	start = i;
	while((*str)[i] && (*str)[i] != ' ' && (*str)[i] != '<' && (*str)[i] != '>')
		i++;
	*dest = ft_strdup_range(*str, start, i);
	start--;
	while(to_clean < i)
	{
		(*str)[to_clean] = ' ';
		to_clean++;
	}
	return (i);
}

int parse(char *str, t_struct *node)
{
	t_struct	*copy;
	int			infile_size;
	int			outfile_size;
	int			i;
	int			k_i;
	int			k_o;

	copy = node;
	i = 0;
	k_i = 0;
	k_o = 0;
	infile_size = number_of_delim(str, '<', 0);
	outfile_size = number_of_delim(str, '>', 1);
	if(infile_size)
		copy->infiles = (char **) malloc(sizeof(char *) * (infile_size + 1));
	if(outfile_size)
		copy->outfiles = (char **) malloc(sizeof(char *) * (outfile_size + 1));
	while(str[i])
	{
		if (str[i] == '\"' || str[i] == '\'')
			i = i + go_to_closing_char(&str[i]) + 1;
		if (str[i] && infile_size > 0 && str[i] == '<' && str[i + 1] != '<')//we reached the infile
		{
			i = save_the_next_word(&str, i + 1, &copy->infiles[k_i], i);
			k_i++;

		}
		else if (str[i] && str[i] == '<' && str[i + 1] == '<')//we reached the heredoc replace by space and skip
		{
			str[i] = ' ';
			str[i + 1] = ' ';
			i = i + 2;
			while(str[i] == ' ')
				i++;
			while(str[i] != ' ' && str[i] != '<' && str[i] != '>' && str[i] != '|')
			{
				str[i] = ' ';
				i++;
			}
		}
		else if (str[i] && str[i] == '>' && str[i + 1] != '>')//we reached the >outfile
		{
			i = save_the_next_word(&str, i + 1, &copy->outfiles[k_o], i);
			k_o++;
		}
		else if (str[i] && str[i] == '>' && str[i + 1] == '>')//we reached the >>outfile
		{
			i = save_the_next_word(&str, i + 2, &copy->outfiles[k_o], i);
			k_o++;
		}
		else
			i++;
	}
	if (infile_size)
		copy->infiles[k_i] = '\0';
	if (outfile_size)
		copy->outfiles[k_o] = '\0';
	copy->cmd = ft_split_custom(str, ' ');//whatever is left in the string is cmd
	remove_double_quotes(copy->cmd);
	return (0);
}

int set_infiles_outfiles_cmds(t_struct **elements)
{
	t_struct *copy;

	copy = *elements;
	while(copy)
	{
		parse(copy->str, copy);
		copy = copy->next;
	}
	return (0);
}

int	remove_double_quotes(char **str)
{
	int i;
	int j;

	i = 0;
	while(str[i])
	{
		j = 0;
		while(str[i][j])
		{
			if (str[i][j] == '\'' || str[i][j] == '\"')
			{
				while (str[i][j])
				{
					str[i][j] = str[i][j + 1];
					j++;
				}
				j = 0;
			}
			j++;
		}
		i++;
	}
	return (0);
}
