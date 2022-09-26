/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution_util.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 19:14:09 by shabibol          #+#    #+#             */
/*   Updated: 2022/09/23 17:02:19 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	file_access_check(char **file, int *file_modes)
{
	int	i;
	int	fd;

	fd = -1;
	i = -1;
	while (file[++i])
	{
		if (fd > 0)
			close(fd);
		if (file_modes[i] == 0)
			fd = open(file[i], O_RDONLY);
		else if (file_modes[i] == 1)
			fd = open(file[i], O_CREAT | O_RDWR | O_TRUNC, 0644);
		else if (file_modes[i] == 2)
			fd = open(file[i], O_CREAT | O_RDWR | O_APPEND, 0644);
		if (fd < 0 && access(file[i], F_OK) == -1)
			return (perror(file[i]), 0);
		else if (fd < 0 && (access(file[i], R_OK) == -1
				|| access(file[i], W_OK) == -1))
			return (perror(file[i]), 0);
		else if (fd < 0)
			return (perror(file[i]), 0);
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
				perror("dup2 stdin:");
			close(head->fds[0]);
		}
	}
	else if (head->fds[0] != STDIN_FILENO)
	{
		if (dup2(head->fds[0], STDIN_FILENO) < 0)
			perror("dup2 stdin:");
		close(head->fds[0]);
	}
}

void	ft_dup2_outfiles(t_struct *head, int *exit_code)
{
	if (head->outfiles && *exit_code != 1)
	{
		head->fds[1] = file_access_check(head->outfiles, head->outfile_modes);
		if (!head->fds[1])
			*exit_code = 1;
		else
		{
			if (dup2(head->fds[1], STDOUT_FILENO) < 0)
				perror("dup2 stdout:");
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

	envp = NULL;
	path_iteri = NULL;
	if (boolean_if_buildin(head->cmd) == 1)
	{
		export_next_cmd(head->cmd[0], envp_head);
		*ec = buildins_dispatch(head->cmd, envp_head);
	}
	else if (head->cmd[0])
	{
		path_iteri = cmd_check(path, ec, head);
		if (path_iteri)
		{
			export_next_cmd(path_iteri, envp_head);
			ft_lst_to_tab(&envp, envp_head);
			execve(path_iteri, head->cmd, envp);
		}
	}
	ft_free_tab(envp);
}
