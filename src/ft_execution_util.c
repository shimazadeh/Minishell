/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 19:14:09 by shabibol          #+#    #+#             */
/*   Updated: 2022/08/24 13:40:53 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cmd_access_check(char **cmd, char **parsed_path)
{
	int		i;
	char	*path_iteri;
	DIR		*stream;

	i = -1;
	stream = NULL;
	while (parsed_path[++i])
	{
		if (ft_strncmp(parsed_path[i], cmd[0], ft_strlen(parsed_path[i])) == 0)
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
				if (!stream && errno == ENOTDIR)
					return (ft_free(path_iteri), 0);
				else if (stream)
					closedir(stream);
			}
			ft_free(path_iteri);
		}
	}
	return (ft_dprintf(2, "%s: command not found\n", cmd[0]), 1);
}

char	*file_access_check(char **file, int flag)
{
	int	i;
	int	fd;

	i = -1;
	while (file[++i])
	{
		if (flag == 0)
			fd = open(file[i], O_RDONLY);
		else if (flag == 1)
			fd = open(file[i], O_CREAT | O_RDWR | O_TRUNC, 0644);
		else if (flag == 2)
			fd = open(file[i], O_CREAT | O_RDWR | O_APPEND, 0644);
		if (fd < 0)
		{
			if (access(file[i], F_OK) == -1)
				perror(file[i]);
			else if (access(file[i], R_OK) == -1 || access(file[i], W_OK) == -1)
				perror(file[i]);
			else
				perror(file[i]);
			return (close(fd), NULL);
		}
		close(fd);
	}
	return (file[i - 1]);
}

void	ft_dup2_infiles(t_struct *head, int *exit_code)
{
	char	*last_infile;

	last_infile = NULL;
	if (head->infiles && head->tag == 0)
	{
		last_infile = file_access_check(head->infiles, 0);
		if (!last_infile)
			*exit_code = 1;
		else
		{
			head->fds[0] = open(last_infile, O_RDONLY);
			if (dup2(head->fds[0], STDIN_FILENO) < 0)
				perror("dup2 stdin inside:");
			close(head->fds[0]);
		}
	}
	else
	{
		if (head->fds[0] != STDIN_FILENO)
		{
			if (dup2(head->fds[0], STDIN_FILENO) < 0)
				perror("dup2 stdin inside:");
			close(head->fds[0]);
		}
	}
}

void	ft_dup2_outfiles(t_struct *head, int *exit_code)
{
	char	*last_outfile;
	int		mode;

	last_outfile = NULL;
	mode = 0;
	if (*exit_code != 1 && head->outfiles)
	{
		mode = head->outfile_modes[ft_strlen_tab(head->outfiles) - 1];
		last_outfile = file_access_check(head->outfiles, mode);
		if (!last_outfile)
			*exit_code = 1;
		else
		{
			if (mode == 1)
				head->fds[1] = open(last_outfile, O_CREAT | O_RDWR | O_TRUNC, 0644);
			else if (mode == 2)
				head->fds[1] = open(last_outfile, O_CREAT | O_RDWR | O_APPEND, 0644);
			if (dup2(head->fds[1], STDOUT_FILENO) < 0)
				perror("dup2 stdout inside:");
			close(head->fds[1]);
		}
	}
	else if (*exit_code != 1)
	{
		if (head->fds[1] != STDOUT_FILENO)
		{
			if (dup2(head->fds[1], STDOUT_FILENO) < 0)
				perror("dup2 stdout:");
			close(head->fds[1]);
		}
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
	else if (cmd_access_check(head->cmd, path) == 0)
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
		*ec = 127;
	}
	ft_free_tab(envp);
}

// int	execute(t_struct **elements, char **parsed_path, t_list **envp)
// {
// 	int			pipefds[2];
// 	t_struct	*copy;
// 	int			exit_code;
// 	int			flag;
// 	int		tmp_stdin;
// 	int		tmp_stdout;

// 	flag = 1;
// 	exit_code = -1;
// 	set_infiles_outfiles_cmds(elements);
// 	copy = *elements;
// 	if (structure_size(*elements) == 1 && boolean_if_buildin(copy->cmd) == 1)
// 		flag = 0;
// 	while (copy)
// 	{
// 		if (copy->next)
// 		{
// 			pipe(pipefds);
// 			copy->fds[1] = pipefds[1];
// 			if (!copy->next->fds[0])
// 				copy->next->fds[0] = pipefds[0];
// 		}
// 		if (flag == 1)
// 			copy->child = fork();
// 		else if(flag == 0)
// 		{
// 			tmp_stdin = dup(STDIN_FILENO);
// 			tmp_stdout = dup(STDOUT_FILENO);
// 		}
// 		exit_code = execute_function(copy, parsed_path, envp, flag);
// 		if (copy->fds[1] != 1)
// 			close(copy->fds[1]);
// 		if (copy->fds[0] != 0)
// 			close(copy->fds[0]);
// 		copy = copy->next;
// 	}
// 	copy = *elements;
// 	if (flag == 1)
// 	{
// 		while (copy)
// 		{
// 			waitpid(copy->child, &(copy->wstatus), 0);
// 			copy = copy->next;
// 		}
// 		exit_code = (0xff00 & structure_last(*elements)->wstatus) >> 8;
// 	}
// 	else
// 	{
// 		if (dup2(tmp_stdin, STDIN_FILENO) < 0)
// 			perror(":");
// 		close(tmp_stdin);
// 		if (dup2(tmp_stdout, STDOUT_FILENO) < 0)
// 			perror(":");
// 		close(tmp_stdout);
// 	}
// 	return (exit_code);
// }
