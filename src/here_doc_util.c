/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 14:15:44 by shabibol          #+#    #+#             */
/*   Updated: 2022/09/23 17:25:14 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_unlink(char **file_names)
{
	int	i;

	i = 0;
	if (!file_names || !*file_names)
		return ;
	while (file_names && file_names[i])
	{
		unlink(file_names[i]);
		i++;
	}
	ft_free_tab(file_names);
	return ;
}

char	**default_name_generator(int size)
{
	int		i;
	char	**file_names;

	i = 0;
	file_names = ft_alloc(sizeof(char *) * (size + 1));
	while (i < size)
	{
		file_names[i] = ft_strjoin("here_doc_", ft_itoa(i));
		i++;
	}
	file_names[i] = NULL;
	return (file_names);
}

char	**fancy_name_generator(int size)
{
	int		i;
	int		fd;
	char	*buf;
	char	*tmp;
	char	**file_names;

	i = -1;
	file_names = ft_alloc(sizeof(char *) * (size + 1));
	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
		file_names = default_name_generator(size);
	else
	{
		while (++i < size)
		{
			buf = get_next_line(fd);
			tmp = ft_strdup_range(buf, 0, 8);
			file_names[i] = ft_strjoin(".", tmp);
			ft_free(tmp);
			ft_free(buf);
		}
		file_names[size] = NULL;
		close(fd);
	}
	return (file_names);
}

int	find_last_infile_type(char *str)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
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
