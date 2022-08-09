/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pipex_Utils_PartI.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shabibol <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 19:14:09 by shabibol          #+#    #+#             */
/*   Updated: 2022/07/05 19:14:10 by shabibol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

void	handle_here_doc(char *str, t_struct **elements)
{
	int i;
	int j;
	int *fds;
	char **file_names;
	int	 *loc;
	char **stop;
	int size;
	t_struct *copy;

	stop = check_for_here_doc(str, &loc);
	size = number_of_here_doc(str);
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
		if (loc[j] = i)
		{
			while (loc[j + 1 ] == i)
				j++;
			copy->fds[0] = fds[j];
		}
		copy = copy->next;
		i++;
	}
}

void	execute(t_struct **elements, char **parsed_path, char **envp, char *str)
{
	int			pipefds[2];
	t_struct	*copy;

	handle_here_doc(str, elements);
	copy = *elements;
	while (copy)
	{
		if (copy->next)
		{
			pipe(pipefds);
			copy->fds[1] = pipefds[1];
			if (!copy->next->fds[0])
				copy->next->fds[0] = pipefds[0];
		}
		copy->child = fork();
		execute_function(copy, parsed_path, envp);
		if (copy->next)
			close(copy->fds[1]);
		if (!(copy == *elements))
			close(copy->fds[0]);
		copy = copy->next;
	}
	copy = *elements;
	while (copy)
	{
		waitpid(copy->child, &(copy->wstatus), 0);
		copy = copy->next;
	}
	return ;
}

void	execute_function(t_struct *head, char **parsed_path, char **envp)
{
	char	*path_iteri;
	int		size;
	char	*last_infile;
	char	*last_outfile;
	int		exit_code;

	size = 0;
	exit_code = 0;
	if (head->child < 0)
		return (perror("Fork:"));
	else if (!(head->child))
	{
		if (parse(head->str, &(head->infiles), '<') == 1 && head->infiles[0][0])
		{
			last_infile = file_access_check(head->infiles, 0); //checks the access of all infiles, returns NULL on failure
			printf("the infile is %s\n", last_infile);
			if (!last_infile) // while on infile + perror if pb
				exit_code = 1;
			else
			{
				head->fds[0] = open(last_infile, O_RDONLY);
				if (dup2(head->fds[0], STDIN_FILENO) < 0)
					perror("dup2 stdin inside:");
			}
		}
		else
		{
			if (dup2(head->fds[0], STDIN_FILENO) < 0)
				perror("dup2 stdin:");
		}
		if (exit_code != 1 && parse(head->str, &(head->outfiles), '>') == 1)
		{
			last_outfile = file_access_check(head->outfiles, 1); //checks the access of all infiles, returns NULL on failure
			if (!last_outfile)
				exit_code = 1;
			else
			{
				head->fds[1] = open(last_outfile, O_CREAT | O_RDWR | O_TRUNC, 0644);
				if (dup2(head->fds[1], STDOUT_FILENO) < 0)
					perror("dup2 stdout inside:");
			}
		}
		else if (exit_code != 1)
		{
			if (dup2(head->fds[1], STDOUT_FILENO) < 0)
				perror("dup2 stdout:");
		}
		if (exit_code != 1 && parse_cmds(head->str, &head->cmd, head) == 1)
		{
		//	dprintf(2, "%s\n", *(head->cmd));
			// if this function is a buuildinf
			// 	blablabla
			// else
			// {
				while (*parsed_path && *(head->cmd))
				{
					size = ft_strlen(*parsed_path);
					if (ft_strncmp(*parsed_path, *(head->cmd), size) != 0)
						path_iteri = ft_strjoin(*parsed_path, *(head->cmd));
					else
						path_iteri = ft_strdup(*(head->cmd));
					execve(path_iteri, head->cmd, envp);
					free(path_iteri);
					(parsed_path)++;
				}
				exit_code = 127;
		}
		exit(exit_code);
	}
}

int parse_cmds(char *str, char ***cmds, t_struct *head)
{
	int i;
	int j;
	int flag;
	(void)head;
	char	*command;

	j = 0;
	i = 0;
	while(str[i] == ' ')
		i++;
	if (str[i] == '<')//all infiles must have < otherwise its a cmd
	{
		flag = 1;
		while(str[i])
		{
			if (str[i] == '<')//the next word is infile
				flag = 1;
			else
				i++;
			while(flag == 1)//pass to the next word
			{
				i++;
				while(str[i] && str[i] == ' ')
					i++;
				while(str[i] && str[i] != ' ')
					i++;
				if (str[i] == ' ')
					flag = 0;
			}
			while(str[i] == ' ')
				i++;
			//dprintf(2, "%d %c\n", flag, str[i]);
			if (flag == 0 && str[i] != '<')//we found the cmd
			{
				j = i;
				while(str[i] && str[i] != '<' && str[i] != '>')
					i++;
				command = ft_strdup_range(str, j, i);
				// dprintf(2, "%s\n", command);
				*cmds = ft_split(command, ' ');
				return (1);
			}
		}
		return (0);
	}
	else if (str[i] != '<')//the first word is a cmd
	{
		j = i;
		while (str[i] && str[i] != '<' && str[i] != '>') // LOGIC !!!!
			i++;
		command = ft_strdup_range(str, j, i);
		// dprintf(2, "%s\n", command);
		*cmds = ft_split(command, ' ');
	}
	return (1);
}

int	all_access_check(t_struct **tab, char **parsed_path)
{
	t_struct	*copy;

	copy = *tab;
	while (copy && access_check(copy->cmd, parsed_path) == 0)
		copy = copy->next;
	if (!copy)
		return (0);
	while (copy)
	{
		if (access_check(copy->cmd, parsed_path) == 1)
			printf("command not found: %s\n", *(copy->cmd));
		copy = copy->next;
	}
	return (close((*tab)->fds[0]), close(sc_lstlast(*tab)->fds[1]), 1);
}


char	**parsing(char *find, char **str)
{
	char	**paths;
	char	*temp;
	char	**tab_temp;
	int		j;
	int		k;

	j = 0;
	paths = (char **)malloc (sizeof(char *) * (ft_strlen(*str) + 1));
	while (*str)
	{
		k = 0;
		temp = ft_strtrim(ft_strnstr(*str, find, ft_strlen(*str)), find);
		if (temp)
		{
			tab_temp = ft_split(temp, ':');
			while (tab_temp && tab_temp[k])
				paths[j++] = ft_strjoin(tab_temp[k++], "/");
			ft_free(tab_temp, k);
			free(temp);
		}
		str++;
	}
	paths[j] = '\0';
	return (paths);
}

char	**find_paths(char *find, char **str)
{
	char	**paths;
	char	*temp;
	char	**tab_temp;
	int		j;
	int		k;

	j = 0;
	paths = (char **)malloc (sizeof(char *) * (ft_strlen(*str) + 1));
	while (*str)
	{
		k = 0;
		temp = ft_strtrim(ft_strnstr(*str, find, ft_strlen(*str)), find);
		if (temp)
		{
			tab_temp = ft_split(temp, ':');
			while (tab_temp && tab_temp[k])
				paths[j++] = ft_strjoin(tab_temp[k++], "/");
			ft_free(tab_temp, k);
			free(temp);
		}
		str++;
	}
	paths[j] = '\0';
	return (paths);
}

char	*file_access_check(char **files, int flag) //0 for infiles, 1 for outfiles OTRUNC, 2 outfiles OAPPEND
{
	int i;
	int fd;

	i = 0;
	while(files[i])
	{
		printf("file is %s\n", files[i]);
		if (flag == 0)
			fd = open(files[i], O_RDONLY);
		else if (flag == 1)
			fd = open(files[i], O_CREAT | O_RDWR | O_TRUNC, 0644);
		else if (flag == 2)
			fd = open(files[i], O_CREAT | O_RDWR | O_APPEND, 0644);
		if (fd < 0)
		{
			if (access(files[i], F_OK) == -1)
				printf("no such file or directory: %s\n", files[i]);
			else if (access(files[i], R_OK) == -1 || access(files[i], W_OK) == -1)
				printf("permission denied: %s\n", files[i]);
			else
				printf("no such file or directory: %s\n", files[i]);
			close(fd);
			return (NULL);
		}
		close(fd);
		i++;
	}
	return (files[i - 1]);
}
