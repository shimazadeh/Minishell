/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shabibol <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 14:15:44 by shabibol          #+#    #+#             */
/*   Updated: 2022/08/24 14:15:46 by shabibol         ###   ########.fr       */
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

	if (size == 0)
		return (NULL);
	i = 0;
	file_names = ft_alloc(sizeof(char *) * (size + 1));
	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
		return (NULL);
	while (i < size)
	{
		buf = get_next_line(fd);
		tmp = ft_strdup_range(buf, 0, 8);
		file_names[i] = ft_strjoin(".", tmp);
		ft_free(tmp);
		ft_free(buf);
		i++;
	}
	file_names[size] = NULL;
	close(fd);
	return (file_names);
}