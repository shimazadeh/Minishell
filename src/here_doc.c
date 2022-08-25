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

int	set_last_infile_type(t_struct **elements, char **file_names, int *loc, int size)
{
	t_struct	*copy;
	int			i;
	int			j;

	i = 0;
	j = 0;
	copy = *elements;
	while (copy)
	{
		if (loc[j] == i && find_last_infile_type(copy->str) == 1)
		{
			while (loc[j] == i && j < size - 1)
				j++;
			dprintf(2, "loc is %d,the str is %s with filename of %s\n", loc[j], copy->str, file_names[j]);
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

void	initialize_heredoc_util_var(t_ft_here_doc_util *v)
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
	t_ft_here_doc_util	v[1];

	initialize_heredoc_util_var(v);
	i = 0;
	k = 0;
	v->str = *str_add;
	v->stop = ft_alloc(sizeof(char *) * (size + 1));
	v->loc = ft_alloc(sizeof(int) * (size));
	while (v->str[i])
	{
		if (v->str[i] == '|')
			v->loc_pipe++;
		if (k < size && v->str[i] == '<' && v->str[i + 1] == '<')
		{
			i = save_the_next_word(&v->str, i + 2, &v->tmp, i);
			v->stop[k] = ft_strjoin(v->tmp, "\n");
			ft_free(v->tmp);
			v->loc[k] = v->loc_pipe;
			dprintf(2, "setting loc for stop %s: %d\n", v->stop[k], v->loc[k]);
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

void	initialize_heredoc_var(t_ft_here_doc *v)
{
	v->file_names = NULL;
	v->stop = NULL;
	v->loc = 0;
	v->str = NULL;
	v->fds = 0;
}

char	**ft_here_doc(char **str_add, t_struct **elements, t_list **envp, int exit)
{
	int		i;
	int		size;
	t_ft_here_doc	v[1];

	initialize_heredoc_var(v);
	v->str = *str_add;
	i = -1;
	size = number_of_here_doc(v->str);
	dprintf(2, "size if %d\n", size);
	if (size == 0)
		return (NULL);
	v->stop = store_heredoc_stops(&v->str, &v->loc, size);
	v->file_names = fancy_name_generator(size);
	v->fds = ft_alloc(sizeof(int) * size);
	if (v->file_names == NULL)
		v->file_names = default_name_generator(size);
	while (v->stop[++i])
		v->fds[i] = write_to_file(v->stop[i], v->file_names[i], envp, exit);
	ft_free(v->stop[i]);
	set_last_infile_type(elements, v->file_names, v->loc, size);
	*str_add = v->str;
	return (v->file_names);
}
