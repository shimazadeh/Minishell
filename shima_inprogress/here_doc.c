

#include "pipex.h"

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
// 		free(str);
// 		str = tmp2;
// 		free(tmp1);
// 		i++;
// 	}
// 	*file_name = ft_split(str, " ");
// }

void	fancy_name_generator(int size, char ***file_names)
{
	int	i;
	int	fd;
	char	*buf;

	*file_names = malloc(sizeof(char *) * (size + 1));
	fd = open("/dev/urandom", O_RDONLY);
	i = 0;
	while (i < size)
	{
		buf = get_next_line(fd);
		*file_names[i] = ft_strjoin(".", ft_strdup_range(buf, 0,  8));
		free(buf);
		i++;
	}
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

char **check_for_here_doc(char *str, int **loc)
{
	int i;
	int start;
	int loc_pipe;
	char **stop;
	int size;
	int k;

	i = 0;
	loc_pipe = 0;
	k = 0;
	size = number_of_here_doc(str);
	if (size == 0)
		return (0);
	stop = malloc(sizeof(char *) * (size + 1));
	*loc = malloc(sizeof(int *) * size);
	while(str[i])
	{
		if (str[i] == '<' && str[i + 1] == '<')
		{
			i = i + 2;
			while(str[i] && str[i] == ' ')
				i++;
			start = i;
			i++;
			while (str[i] && str[i] != ' ')
				i++;
			stop[k] = ft_strjoin(ft_strdup_range(str, start, i), "\n");
			*loc[k] = loc_pipe;
			k++;
		}
		if (str[i] == '|')
			loc_pipe++;
		i++;
	}
	return (stop);
}

void	handle_here_doc(char *str, t_struct **elements)
{
	int 	i;
	int 	j;
	int 	*fds;
	char 	**file_names;
	int	 	*loc;
	char 	**stop;
	int 	size;
	t_struct *copy;

	stop = check_for_here_doc(str, &loc);
	size = number_of_here_doc(str);
	if (size == 0)
		return ;
	fds = malloc(sizeof(int) * size);
	fancy_name_generator(size, &file_names);
	i = 0;
	while(stop[i])
	{
		write_to_file(fds[i], stop[i], file_names[i]);
		i++;
	}
	copy = *elements;
	i = 0;
	j = 0;
	while(copy)
	{
		if (loc[j] == i && find_last_infile_type(copy->str) == 1)//if the last infile in the string is here_doc then set the fd
		{
			while (loc[j + 1] && loc[j + 1] == i)
				j++;
			copy->fds[0] = open(file_names[j], O_RDONLY, 0777);
			// copy->fds[0] = fds[j];
			copy->tag = 1;//indicates that the last infile is a heredoc type
		}
		copy = copy->next;
		i++;
	}
}
