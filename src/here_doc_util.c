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

int	write_to_file(char *stop, char	*file, t_list **envp, int last_exit_code)
{
	char	*gnl;
	int		fd1;
	char	*stop_new;

	stop_new = ft_strjoin(stop, "\n");
	fd1 = open(file, O_CREAT | O_RDWR, 0777);
	if (fd1 < 0)
		return (ft_dprintf(2, "error creating here_doc\n"), -1);
	write(1, "> ", 2);
	gnl = get_next_line(0);
	while (!(g_var->sig_flag) && gnl
		&& ft_strncmp(gnl, stop_new, ft_strlen(stop_new) + 1) != 0)
	{
		gnl = variable_expansion_hd(&gnl, envp, last_exit_code);
		if (write(fd1, gnl, ft_strlen(gnl)) < 0)
			return (perror("write:"), -1);
		ft_free(gnl);
		write(1, "> ", 2);
		gnl = get_next_line(0);
	}
	if (!gnl)
		ft_dprintf(1,
			"warning: here-document delimited by end-of-file (wanted `%s')\n",
			stop);
	return (close(fd1), ft_free(gnl), ft_free(stop), ft_free(stop_new), fd1);
}

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
