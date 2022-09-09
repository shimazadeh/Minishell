/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 16:54:40 by shabibol          #+#    #+#             */
/*   Updated: 2022/09/08 18:26:59 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_last_infile_type(t_struct **elements, char **file, int *loc, int size)
{
	t_struct	*copy;
	int			i;
	int			j;
	int			tmp;

	i = 0;
	j = 0;
	tmp = 0;
	copy = *elements;
	if (g_var->sig_flag)
		return (1);
	while (copy)
	{
		if (loc[j] == i && find_last_infile_type(copy->str) == 1)
		{
			tmp = j;
			while (++j < size - 1 && loc[j] == i)
				tmp = j;
			copy->fds[0] = open(file[tmp], O_RDONLY, 0777);
			copy->tag = 1;
		}
		copy = copy->next;
		i++;
	}
	return (ft_free(loc), 0);
}

char	**store_heredoc_stops(char **str_add, int *flag, int size)
{
	int		i;
	int		k;
	char	**stop;
	char	*str;

	i = 0;
	k = 0;
	str = *str_add;
	stop = ft_alloc(sizeof(char *) * (size + 1));
	while (str[i])
	{
		if (k < size && str[i] == '<' && str[i + 1] == '<')
		{
			if (quotes_presence(&str[i + 2]) == 1)
				flag[k] = 1;
			else
				flag[k] = 0;
			i = save_next_word(&str, i + 2, &stop[k], i - 1);
			k++;
		}
		else
			i++;
	}
	stop[k] = '\0';
	return (*str_add = str, stop);
}

int	*store_heredoc_loc(char *str, int size)
{
	int		i;
	int		k;
	int		*loc;
	int		loc_pipe;

	i = 0;
	k = 0;
	loc_pipe = 0;
	loc = ft_alloc(sizeof(int) * (size));
	while (str[i])
	{
		if (str[i] == '|')
			loc_pipe++;
		if (k < size && str[i] == '<' && str[i + 1] == '<')
		{
			loc[k] = loc_pipe;
			k++;
			i = pass_the_next_word(&str[i + 2]) + 2;
		}
		else
			i++;
	}
	return (loc);
}

void	initialize_heredoc_var(t_ft_here_doc *v)
{
	v->file_names = NULL;
	v->stop = NULL;
	v->exp_flags = 0;
	v->loc = 0;
	v->str = NULL;
	v->fds = 0;
}

char	**ft_here_doc(char **str_add, t_struct **sc, t_list **envp, int exit)
{
	int				i;
	int				size;
	t_ft_here_doc	v[1];

	initialize_heredoc_var(v);
	v->str = *str_add;
	i = -1;
	size = number_of_here_doc(v->str);
	if (size == 0)
		return (NULL);
	v->loc = store_heredoc_loc(v->str, size);
	v->exp_flags = ft_alloc(sizeof(int) * size);
	v->stop = store_heredoc_stops(&v->str, v->exp_flags, size);
	v->fds = ft_alloc(sizeof(int) * size);
	v->file_names = fancy_name_generator(size);
	if (v->file_names == NULL)
		v->file_names = default_name_generator(size);
	while (g_var->sig_flag == 0 && v->stop[++i])
		v->fds[i] = write_to_file(v, i, envp, exit);
	ft_free(v->stop[i]);
	set_last_infile_type(sc, v->file_names, v->loc, size);
	*str_add = v->str;
	return (v->file_names);
}
