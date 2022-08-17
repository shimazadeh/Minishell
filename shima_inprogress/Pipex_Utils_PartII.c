/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pipex_Bonus_Utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shabibol <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 17:10:45 by shabibol          #+#    #+#             */
/*   Updated: 2022/07/02 17:10:50 by shabibol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**glob_free(char **dst)
{
	int	i;

	i = 0;
	if (!dst)
		return (NULL);
	while (dst && dst[i])
		i++;
	while (i > 0)
	{
		i--;
		free(dst[i]);
	}
	free(dst);
	return (NULL);
}

void	ft_free_lst(t_struct *lst)
{
	t_struct	*tmp;

	if (!lst)
		return ;
	while (lst)
	{
		glob_free(lst->cmd);
		glob_free(lst->infiles);
		glob_free(lst->outfiles);
		free(lst->str);
		tmp = lst;
		lst = lst->next;
		free(tmp);
	}
	return ;
}


int	write_to_file(int fd1, char *stop, char	*file_name)
{
	char	*gnl;

	fd1 = open(file_name, O_CREAT | O_RDWR, 0777);
	if (fd1 < 0)
		return (printf("error with creating here_doc\n"), -1);
	write(1, "> ", 2);
	gnl = get_next_line(0);
	while (gnl && ft_strncmp(gnl, stop, ft_strlen(stop) + 1) != 0)
	{
		write(1, "> ", 2);
		if (write(fd1, gnl, ft_strlen(gnl)) < 0)
		{
			perror("write:");
			return (-1);
		}
		free(gnl);
		gnl = get_next_line(0);
	}
	free(gnl);
	close(fd1);
	return (fd1);
}

int	access_check(char **cmd, char **parsed_path)
{
	int		i;
	char	*path_iteri;

	i = 0;
	while (parsed_path[i])
	{
		if (cmd[0] && ft_strncmp(parsed_path[i], cmd[0], ft_strlen(parsed_path[i])) == 0)
		{
			if (access(cmd[0], F_OK) == 0 && access(cmd[0], X_OK) == 0)
				return (0);
		}
		else
		{
			path_iteri = ft_strjoin(parsed_path[i], cmd[0]);
			if (access(path_iteri, F_OK) == 0 && access(path_iteri, X_OK) == 0)
				return (free(path_iteri), 0);
			free(path_iteri);
		}
		i++;
	}
	write(2, "command not found ", 18); //replace these with Adrien's dprintf
	write(2, cmd[0], ft_strlen(cmd[0]));
	write(2, "\n", 1);
	return (1);
}
