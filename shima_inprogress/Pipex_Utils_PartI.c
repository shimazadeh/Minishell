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

void	execute(t_struct **elements, char **parsed_path, char **envp, char *str)
{
	int			pipefds[2];
	t_struct	*copy;

	handle_here_doc(str, elements);
	set_infiles_outfiles_cmds(elements);
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
		// dprintf(2, "the tag is %d\n", copy->tag);
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
		dprintf(2, "the infile is %s\n", head->infiles[0]);
		if (head->infiles[0] && head->tag == 0)
		{
			last_infile = file_access_check(head->infiles, 0); //checks the access of all infiles, returns NULL on failure
			dprintf(2, "the last infile is %s\n", last_infile);
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
			dprintf(2, "inside the else if of infiles\n");
			if (dup2(head->fds[0], STDIN_FILENO) < 0)
				perror("dup2 stdin:");
		}
		if (exit_code != 1 && head->outfiles[0])
		{
			last_outfile = file_access_check(head->outfiles, 1); //checks the access of all infiles, returns NULL on failure
			dprintf(2, "the last outfile is %s\n", last_outfile);
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
			dprintf(2, "Im inside the else if of outfile\n");
			if (dup2(head->fds[1], STDOUT_FILENO) < 0)
				perror("dup2 stdout:");
		}
		dprintf(2, "the cmd is %s\n", head->cmd[0]);
		if (exit_code != 1 && head->cmd[0])
		{
			// exit_code = buildin_dispatch(head->cmd, envp_head);
			// if (exit_code == 127)
			// {
				if (access_check(head->cmd, parsed_path) == 0)
				{
					while (*parsed_path && head->cmd[0])
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
			// }
		}
		exit(exit_code);
	}
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
		// dprintf(2, "file is %s\n", files[i]);
		if (flag == 0)
			fd = open(files[i], O_RDONLY);
		else if (flag == 1)
			fd = open(files[i], O_CREAT | O_RDWR | O_TRUNC, 0644);
		else if (flag == 2)
			fd = open(files[i], O_CREAT | O_RDWR | O_APPEND, 0644);
		if (fd < 0)
		{
			if (access(files[i], F_OK) == -1)//no such file or directory
				perror(files[i]);
			else if (access(files[i], R_OK) == -1 || access(files[i], W_OK) == -1)//permission denied
				perror(files[i]);
			else//file doesnt exist
				perror(files[i]);
			close(fd);
			return (NULL);
		}
		close(fd);
		i++;
	}
	return (files[i - 1]);
}
