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

int find_last_infile_type(char *str)//returns 0 if the last infile is an actual file, 1 if here_doc
{
	int i;
	int flag;

	i = 0;
	while(str[i])
	{
		if (str[i] == '<' && str[i + 1] == '<')
		{
			i++;
			flag = 1;//its a here_doc
		}
		else if (str[i] == '<' && str[i + 1] != '<')
			flag = 0;//its an infile
		i++;
	}
	return (flag);
}

// void	default_name_generator(int size, char ***file_names)
// {
// 	int	i;
// 	char *str;
// 	char *tmp1;
// 	char *tmp2;

// 	i = 0;
// 	while (i < size)
// 	{
// 		tmp1 = ft_strjoin("here_doc_", itoa(i));
// 		tmp2 = ft_strjoin(tmp1, " ");
// 		ft_free(str);
// 		str = tmp2;
// 		ft_free(tmp1);
// 		i++;
// 	}
// 	*file_name = ft_split(str, " ");
// }

char	**fancy_name_generator(int size)
{
	int	i;
	int	fd;
	char	*buf;
	char	*tmp;
	char	**file_names;

	if (size == 0)
		return (NULL);
	file_names = ft_alloc(sizeof(char *) * (size + 1));
	fd = open("/dev/urandom", O_RDONLY);
	i = 0;
	// dprintf(2, "size is %d\n", size);
	while (i < size)
	{
		buf = get_next_line(fd);
		tmp = ft_strdup_range(buf, 0 , 8);
		file_names[i] = ft_strjoin(".", tmp);
		ft_free(tmp);
		ft_free(buf);
		i++;
	}
	file_names[size] = NULL;
	close(fd);
	return (file_names);
}

int number_of_here_doc(char *str)
{
	int i;
	int count;

	count = 0;
	i = 0;
	while(str[i])
	{
		if (str[i] == '<' && str[i + 1] == '<')
			count++;
		i++;
	}
	return (count);
}

char **check_for_here_doc(char *str, int **loc_add)
{
	int i;
	int start;
	int loc_pipe;
	char **stop;
	int size;
	int k;
	char *tmp;
	int *loc;

	i = 0;
	loc_pipe = 0;
	k = 0;
	size = number_of_here_doc(str);
	if (size == 0)
		return (0);
	stop = ft_alloc(sizeof(char *) * (size + 1));
	loc = ft_alloc(sizeof(int) * (size));
	while(str[i])
	{
		if (k < size && str[i] == '<' && str[i + 1] == '<')
		{
			i = i + 2;
			while(str[i] && str[i] == ' ')
				i++;
			start = i;
			i++;
			while (str[i] && str[i] != ' ')
				i++;
			tmp = ft_strdup_range(str, start, i);
			stop[k] = ft_strjoin(tmp, "\n");
			ft_free(tmp);
			loc[k] = loc_pipe;
			k++;
		}
		if (str[i] == '|')
			loc_pipe++;
		i++;
	}
	stop[k] = '\0';
	*loc_add = loc;
	return (stop);
}

char	**handle_here_doc(char *str, t_struct **elements, t_list **envp_head, int last_exit_code)
{
	int 	i;
	int 	j;
	int 	*fds;
	char 	**file_names;
	char 	**stop;
	int 	size;
	t_struct *copy;
	int	 	*loc;

	loc = NULL;
	size = number_of_here_doc(str);
	if (size == 0)
		return (NULL);
	stop = check_for_here_doc(str, &loc);
	fds = ft_alloc(sizeof(int) * size);
	file_names = fancy_name_generator(size);
	i = 0;
	while(stop[i])
	{
		// dprintf(2, "stop[i] is %s, filename is %s\n", stop[i], file_names[i]);
		write_to_file(fds[i], stop[i], file_names[i], envp_head, last_exit_code);
		i++;
	}
	ft_free(stop[i]);
	copy = *elements;
	i = 0;
	j = 0;
	while(copy)
	{
		if (loc[j] == i && find_last_infile_type(copy->str) == 1)//if the last infile in the string is here_doc then set the fd
		{
			while (j < size - 1)
				j++;
			// dprintf(2, "j is %d\n", j);
			copy->fds[0] = open(file_names[j], O_RDONLY, 0777);
			// copy->fds[0] = fds[j];
			copy->tag = 1;//indicates that the last infile is a heredoc type
		}
		copy = copy->next;
		i++;
	}
	ft_free_tab(stop);
	// ft_free_tab(file_names);
	ft_free(fds);
	ft_free(loc);
	return(file_names);
}

void	ft_unlink(char **file_names)
{
	int i;

	i = 0;
	if (!file_names || !*file_names)
		return ;
	while(file_names && file_names[i])
	{
		unlink(file_names[i]);
		i++;
	}
	ft_free_tab(file_names);
	return ;
}
