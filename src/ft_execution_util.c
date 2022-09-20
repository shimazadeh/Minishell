/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution_util.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 19:14:09 by shabibol          #+#    #+#             */
/*   Updated: 2022/09/20 16:04:46 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cmd_access_check(char **cmd, char **parsed_path, int *last_exit_code)
{
	int		i;
	char	*path_iteri;
	DIR		*stream;

	stream = NULL;
	i = -1;
	path_iteri = NULL;
	while (parsed_path[++i])
	{
		path_iteri = ft_strjoin(parsed_path[i], cmd[0]);
		if (ft_strncmp(parsed_path[i], cmd[0], ft_strlen(parsed_path[i])) == 0 \
			&& access(cmd[0], F_OK) == 0 && access(cmd[0], X_OK) == 0)
			return (0);
		else if (access(path_iteri, F_OK) == 0 && access(path_iteri, X_OK) == 0)
		{
			stream = opendir(path_iteri);
			if (!stream && errno == ENOTDIR)
				return (ft_free(path_iteri), 0);
			else if (stream)
				closedir(stream);
		}
		ft_free(path_iteri);
	}
	ft_dprintf(2, "%s: command not found\n", cmd[0]);
	return (*last_exit_code = 127, 1);
}

int	file_access_check(char **file, int *file_modes)
{
	int	i;
	int	fd;

	fd = -1;
	i = -1;
	while (file[++i])
	{
		close(fd);
		if (file_modes[i] == 0)
			fd = open(file[i], O_RDONLY);
		else if (file_modes[i] == 1)
			fd = open(file[i], O_CREAT | O_RDWR | O_TRUNC, 0644);
		else if (file_modes[i] == 2)
			fd = open(file[i], O_CREAT | O_RDWR | O_APPEND, 0644);
		if (fd < 0 && access(file[i], F_OK) == -1)
			return (perror(file[i]), close(fd), 0);
		else if (fd < 0 && (access(file[i], R_OK) == -1
				|| access(file[i], W_OK) == -1))
			return (perror(file[i]), close(fd), 0);
		else if (fd < 0)
			return (perror(file[i]), close(fd), 0);
	}
	return (fd);
}

void	ft_dup2_infiles(t_struct *head, int *exit_code)
{
	if (head->infiles && head->tag == 0)
	{
		head->fds[0] = file_access_check(head->infiles, head->infile_modes);
		if (!head->fds[0])
			*exit_code = 1;
		else
		{
			if (dup2(head->fds[0], STDIN_FILENO) < 0)
				perror("dup2 stdin inside:");
			close(head->fds[0]);
		}
	}
	else if (head->fds[0] != STDIN_FILENO)
	{
		if (dup2(head->fds[0], STDIN_FILENO) < 0)
			perror("dup2 stdin inside:");
		close(head->fds[0]);
	}
}

void	ft_dup2_outfiles(t_struct *head, int *exit_code)
{
	if (*exit_code != 1 && head->outfiles)
	{
		head->fds[1] = file_access_check(head->outfiles, head->outfile_modes);
		if (!head->fds[1])
			*exit_code = 1;
		else
		{
			if (dup2(head->fds[1], STDOUT_FILENO) < 0)
				perror("dup2 stdout inside:");
			close(head->fds[1]);
		}
	}
	else if (*exit_code != 1 && head->fds[1] != STDOUT_FILENO)
	{
		if (dup2(head->fds[1], STDOUT_FILENO) < 0)
			perror("dup2 stdout:");
		close(head->fds[1]);
	}
}

void	ft_execute_cmd(t_struct *head, int *ec, char **path, t_list **envp_head)
{
	char	**envp;
	char	*path_iteri;
	int		size;

	size = 0;
	envp = NULL;
	path_iteri = NULL;
	ft_lst_to_tab(&envp, envp_head);
	if (boolean_if_buildin(head->cmd) == 1)
		*ec = buildins_dispatch(head->cmd, envp_head);
	else if (head->cmd[0] && cmd_access_check(head->cmd, path, ec) == 0)
	{
		while (*path && head->cmd[0])
		{
			size = ft_strlen(*path);
			if (ft_strncmp(*path, *(head->cmd), size) != 0)
				path_iteri = ft_strjoin(*path, *(head->cmd));
			else
				path_iteri = ft_strdup(*(head->cmd));
			execve(path_iteri, head->cmd, envp);
			(path)++;
		}
	}
	ft_free_tab(envp);
}
