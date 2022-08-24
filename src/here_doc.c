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

char	**store_heredoc_stops(char **str_add, int **loc_add, int size)
{
	int		i;
	int		loc_pipe;
	char	**stop;
	int		k;
	char	*tmp;
	int		*loc;
	char	*str;

	i = 0;
	loc_pipe = 0;
	k = 0;
	str = *str_add;
	stop = ft_alloc(sizeof(char *) * (size + 1));
	loc = ft_alloc(sizeof(int) * (size));
	while (str[i])
	{
		if (str[i] == '|')
			loc_pipe++;
		if (k < size && str[i] == '<' && str[i + 1] == '<')
		{
			i = save_the_next_word(&str, i + 2, &tmp, i);
			stop[k] = ft_strjoin(tmp, "\n");
			ft_free(tmp);
			loc[k] = loc_pipe;
			k++;
		}
		else
			i++;
	}
	stop[k] = '\0';
	*loc_add = loc;
	*str_add = str;
	return (stop);
}

char	**ft_here_doc(char **str_add, t_struct **elements, t_list **envp, int exit)
{
	int		i;
	char	**file_names;
	char	**stop;
	int		size;
	int		*loc;
	char	*str;

	str = *str_add;
	loc = NULL;
	i = -1;
	size = number_of_here_doc(str);
	if (size == 0)
		return (NULL);
	stop = store_heredoc_stops(&str, &loc, size);
	file_names = fancy_name_generator(size);
	if (file_names == NULL)
		file_names = default_name_generator(size);
	while (stop[++i])
		write_to_file(stop[i], file_names[i], envp, exit);
	ft_free(stop[i]);
	set_last_infile_type(*elements, file_names, loc, size);
	*str_add = str;
	return (file_names);
}
