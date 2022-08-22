/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 17:10:45 by shabibol          #+#    #+#             */
/*   Updated: 2022/08/22 20:07:13 by aguillar         ###   ########.fr       */
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

int	write_to_file(int fd1, char *stop, char	*file_name, t_list **envp_head, int last_exit_code)
{
	char	*gnl;

	fd1 = open(file_name, O_CREAT | O_RDWR, 0777);
	if (fd1 < 0)
		return (printf("error with creating here_doc\n"), -1);
	write(1, "> ", 2);
	gnl = get_next_line(0);
	while (gnl && ft_strncmp(gnl, stop, ft_strlen(stop) + 1) != 0)
	{
		// dprintf(2, "before expansion %s\n", gnl);
		variable_expansion(&gnl, envp_head, last_exit_code);
		// dprintf(2, "after expansion %s\n", gnl);
		if (write(fd1, gnl, ft_strlen(gnl)) < 0)
		{
			perror("write:");
			return (-1);
		}
		ft_free(gnl);
		write(1, "> ", 2);
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
	DIR		*stream;

	i = 0;
	stream = NULL;
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
			{
				stream = opendir(path_iteri);
				if (!stream)
				{
					if (errno == ENOTDIR)
						return (ft_free(path_iteri), 0);
				}
				else
					closedir(stream);
			}

			ft_free(path_iteri);
		}
		i++;
	}
	ft_dprintf(2, "%s: command not found\n", cmd[0]);
	return (1);
}
