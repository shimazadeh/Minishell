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

int save_the_next_word(char **str_add, int i, char **dest, int to_clean)
{
	int start;
	char end;
	char *str;

	str = *str_add;
	end = ' ';
	while(str[i] && str[i] == ' ')
		i++;
	if (str[i] == '\'' || str[i] == '\"')
	{
		end = str[i];
		str[i] = ' ';
		i++;
	}
	start = i;
	while(str[i] && str[i] != end && str[i] != '<' && str[i] != '>')
		i++;
	*dest = ft_strdup_range(str, start, i);
	if (str[i] == end)
		str[i] = ' ';
	start--;
	while(to_clean < i)
	{
		str[to_clean] = ' ';
		to_clean++;
	}
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
	char		end;

	i = 0;
	k = 0;
	end = ' ';
	str = *str_add;
	dest_size = number_of_delim(str, '<', 0);
	if(!dest_size)
		dest = NULL;//I still need to check the here_doc
	else
		dest = (char **)ft_alloc(sizeof(char *) * (dest_size + 1));
	while(str[i])
	{
		// dprintf(2, "str[i] %c\n", str[i]);
		if (str[i] == '\"' || str[i] == '\'')
			i = i + go_to_closing_char(&str[i]) + 1;//if there is no match of closing char return
		else if (str[i] && str[i] == '<' && str[i + 1] != '<')//we reached the infile
		{
			i = save_the_next_word(&str, i + 1, &dest[k], i);
			k++;
		}
		else if (str[i] && str[i] == '<' && str[i + 1] == '<')//we reached the heredoc replace by space and skip
		{
			str[i] = ' ';
			str[i + 1] = ' ';
			i = i + 2;
			while(str[i] && str[i] == ' ')
				i++;
			if (str[i] && (str[i] == '\"' || str[i] == '\''))
			{
				end = str[i];
				str[i] = ' ';
				i++;
			}
			while(str[i] && str[i] != end && str[i] != '<' && str[i] != '>' && str[i] != '|')
			{
				str[i] = ' ';
				i++;
			}
			if (str[i] && (str[i] == '\"' || str[i] == '\''))
				str[i] = ' ';
		}
		else
			i++;
	}
	if (dest_size)
		dest[k] = '\0';
	*str_add = str;
	return (dest);
}

char	**parse_outfiles(char **str_add)
{
	int			dest_size;
	char		**dest;
	char		*str;
	int			i;
	int			k;

	i = 0;
	k = 0;
	str = *str_add;
	dest_size = number_of_delim(str, '>', 1);
	if(!dest_size)
		return (NULL);
	dest = (char **) ft_alloc(sizeof(char *) * (dest_size + 1));
	while(str[i])
	{
		if (str[i] == '\"' || str[i] == '\'')
			i = i + go_to_closing_char(&str[i]) + 1;//if there is no match of closing char return
		else if(str[i] && str[i] == '>')
		{
			if (str[i + 1] != '>')//we reached the >outfile
				i = save_the_next_word(&str, i + 1, &dest[k], i);
			else if (str[i + 1] == '>')//we reached the >>outfile
				i = save_the_next_word(&str, i + 2, &dest[k], i);
			k++;
		}
		else
			i++;
	}
	dest[k] = '\0';
	*str_add = str;
	return (dest);
}

int set_infiles_outfiles_cmds(t_struct **elements)
{
	t_struct *copy;

	copy = *elements;
	while(copy)
	{
		dprintf(2,"str is: %s\n", copy->str);
		copy->infiles = parse_infiles(&copy->str);
		dprintf(2, "str in parsing before split/infile: %s\n", copy->str);
		copy->outfiles = parse_outfiles(&copy->str);
		dprintf(2, "str in parsing before split/outfile: %s\n", copy->str);
		copy->cmd = ft_split_custom(copy->str, ' ', 1);
		handle_wildcards(&copy->cmd);
		copy = copy->next;
	}
	return (0);
}

// int parse(char *str, t_struct *node)
// {
// 	t_struct	*copy;
// 	int			infile_size;
// 	int			outfile_size;
// 	int			i;
// 	int			k_i;
// 	int			k_o;

// 	copy = node;
// 	i = 0;
// 	k_i = 0;
// 	k_o = 0;
// 	infile_size = number_of_delim(str, '<', 0);
// 	outfile_size = number_of_delim(str, '>', 1);
// 	if(infile_size)
// 		copy->infiles = (char **) ft_alloc(sizeof(char *) * (infile_size + 1));
// 	if(outfile_size)
// 		copy->outfiles = (char **) ft_alloc(sizeof(char *) * (outfile_size + 1));
// 	while(str[i])
// 	{
// 		if (str[i] == '\"' || str[i] == '\'')
// 			i = i + go_to_closing_char(&str[i]) + 1;//if there is no match of closing char return
// 		if (str[i] && infile_size > 0 && str[i] == '<' && str[i + 1] != '<')//we reached the infile
// 		{
// 			// dprintf(2, "the str is : --%s--\n", str);
// 			i = save_the_next_word(&str, i + 1, &copy->infiles[k_i], i);
// 			// dprintf(2, "infile is ---%s---\n", copy->infiles[k_i]);
// 			k_i++;
// 		}
// 		else if (str[i] && str[i] == '<' && str[i + 1] == '<')//we reached the heredoc replace by space and skip
// 		{
// 			str[i] = ' ';
// 			str[i + 1] = ' ';
// 			i = i + 2;
// 			while(str[i] == ' ')
// 				i++;
// 			while(str[i] != ' ' && str[i] != '<' && str[i] != '>' && str[i] != '|')
// 			{
// 				str[i] = ' ';
// 				i++;
// 			}
// 		}
// 		else if (str[i] && str[i] == '>' && str[i + 1] != '>')//we reached the >outfile
// 		{
// 			i = save_the_next_word(&str, i + 1, &copy->outfiles[k_o], i);
// 			k_o++;
// 		}
// 		else if (str[i] && str[i] == '>' && str[i + 1] == '>')//we reached the >>outfile
// 		{
// 			i = save_the_next_word(&str, i + 2, &copy->outfiles[k_o], i);
// 			k_o++;
// 		}
// 		else
// 			i++;
// 	}
// 	if (infile_size)
// 		copy->infiles[k_i] = '\0';
// 	if (outfile_size)
// 		copy->outfiles[k_o] = '\0';
// 	copy->cmd = ft_split_custom(str, ' ');//whatever is left in the string is cmd
// 	handle_wildcards(&copy->cmd);
// 	return (0);
// }
