/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shabibol <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 16:54:40 by shabibol          #+#    #+#             */
/*   Updated: 2022/08/17 16:54:42 by shabibol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_last_infile_type(char *str)
{
	int	i;
	int	flag;

	i = 0;
	while (str[i])
	{
		if (str[i] == '<' && str[i + 1] == '<')
		{
			i++;
			flag = 1;
		}
		else if (str[i] == '<' && str[i + 1] != '<')
			flag = 0;
		i++;
	}
	return (flag);
}

int	set_last_infile_type(t_struct *head, char **file_names, int *loc, int size)
{
	t_struct	*copy;
	int			i;
	int			j;

	i = 0;
	j = 0;
	copy = head;
	while (copy)
	{
		if (loc[j] == i && find_last_infile_type(copy->str) == 1)
		{
			while (j < size - 1)
				j++;
			copy->fds[0] = open(file_names[j], O_RDONLY, 0777);
			copy->tag = 1;
		}
		copy = copy->next;
		i++;
	}
	ft_free(loc);
	return (0);
}

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

void	initialize_heredoc_util_var(s_ft_here_doc_util *v)
{
	v->loc_pipe = 0;
	v->stop = NULL;
	v->loc = 0;
	v->str = NULL;
	v->tmp = NULL;
}

char	**store_heredoc_stops(char **str_add, int **loc_add, int size)
{
	int		i;
	int		k;
	s_ft_here_doc_util	v[1];

	initialize_heredoc_util_var(v);
	i = 0;
	k = 0;
	v->str = *str_add;
	v->stop = ft_alloc(sizeof(char *) * (size + 1));
	v->loc = ft_alloc(sizeof(int) * (size));
	while (v->str[i])
	{
		if (v->str[i] == '|')
			loc_pipe++;
		if (k < size && v->str[i] == '<' && v->str[i + 1] == '<')
		{
			i = save_the_next_word(&v->str, i + 2, &v->tmp, i);
			v->stop[k] = ft_strjoin(v->tmp, "\n");
			ft_free(v->tmp);
			v->loc[k] = loc_pipe;
			k++;
		}
		else
			i++;
	}
	v->stop[k] = '\0';
	*loc_add = v->loc;
	*str_add = v->str;
	return (v->stop);
}

void	initialize_heredoc_var(s_ft_here_doc *v)
{
	v->file_names = NULL;
	v->stop = NULL;
	v->loc = 0;
	v->str = NULL;
}

char	**ft_here_doc(char **str_add, t_struct **elements, t_list **envp, int exit)
{
	int		i;
	int		size;
	s_ft_here_doc	v[1];

	initialize_heredoc_var(v);
	v->str = *str_add;
	i = -1;
	size = number_of_here_doc(str);
	if (size == 0)
		return (NULL);
	v->stop = store_heredoc_stops(&str, &loc, size);
	v->file_names = fancy_name_generator(size);
	if (v->file_names == NULL)
		v->file_names = default_name_generator(size);
	while (v->stop[++i])
		write_to_file(v->stop[i], v->file_names[i], envp, exit);
	ft_free(v->stop[i]);
	set_last_infile_type(*elements, v->file_names, v->loc, size);
	*str_add = v->str;
	return (file_names);
}
