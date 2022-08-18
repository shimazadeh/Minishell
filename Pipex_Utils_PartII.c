/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pipex_Utils_PartII.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 17:10:45 by shabibol          #+#    #+#             */
/*   Updated: 2022/08/18 19:34:14 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_sc(t_struct *lst)
{
	t_struct	*tmp;

	if (!lst)
		return ;
	while (lst)
	{
		ft_free_tab(lst->cmd);
		ft_free_tab(lst->infiles);
		ft_free_tab(lst->outfiles);
		ft_free(lst->str);
		tmp = lst;
		lst = lst->next;
		ft_free(tmp);
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
		ft_free(gnl);
		gnl = get_next_line(0);
	}
	ft_free(gnl);
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
				return (ft_free(path_iteri), 0);
			ft_free(path_iteri);
		}
		i++;
	}
	write(2, "command not found ", 18); //replace these with Adrien's dprintf
	write(2, cmd[0], ft_strlen(cmd[0]));
	write(2, "\n", 1);
	return (1);
}
