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

int	boolean_if_buildin(char **av)
{
	if (!av || !av[0])
		return (-1);
	if (!ft_strncmp(av[0], "echo", 5))
		return (1);
	if (!ft_strncmp(av[0], "cd", 3) && (!av[1] || (av[1] && !av[2])))
		return (1);
	if (!ft_strncmp(av[0], "pwd", 4))
		return (1);
	if (!ft_strncmp(av[0], "export", 7))
		return (1);
	if (!ft_strncmp(av[0], "unset", 6))
		return (1);
	if (!ft_strncmp(av[0], "env", 4) && !av[1])
		return (1);
	if (!ft_strncmp(av[0], "exit", 5))
		return (1);
	return (0);
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

void	ft_dup2_infiles(t_struct *head, int *exit_code)
{
	char *last_infile;

	last_infile = NULL;
	if (head->infiles && head->tag == 0)
	{
		last_infile = file_access_check(head->infiles, 0); //checks the access of all infiles, returns NULL on failure
		if (!last_infile) // while on infile + perror if pb
			*exit_code = 1;
		else
		{
			head->fds[0] = open(last_infile, O_RDONLY);
			// if (head->fds[0] != STDIN_FILENO)
			// {
				if (dup2(head->fds[0], STDIN_FILENO) < 0)
					perror("dup2 stdin inside:");
				close(head->fds[0]);
			// }
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
	char *last_outfile;

	last_outfile = NULL;
	if (exit_code != 1 && head->outfiles)
	{
		last_outfile = file_access_check(head->outfiles, 1); //checks the access of all outfiles, returns NULL on failure
		if (!last_outfile)
			*exit_code = 1;
		else
		{
			head->fds[1] = open(last_outfile, O_CREAT | O_RDWR | O_TRUNC, 0644);
			// if (head->fds[1]  != STDOUT_FILENO)
			// {
				if (dup2(head->fds[1], STDOUT_FILENO) < 0)
					perror("dup2 stdout inside:");
				close(head->fds[1]);
			// }
		}
	}
	else if (exit_code != 1)
	{
		if (head->fds[1] != STDOUT_FILENO)
		{
			if (dup2(head->fds[1], STDOUT_FILENO) < 0)
				perror("dup2 stdout:");
			close(head->fds[1]);
		}
	}
}

void	ft_execute_cmd(t_struct *head, int *exit_code, char **parsed_path, t_list **envp_head)
{
	char	**envp;
	char	*path_iteri;
	int		size;

	size = 0;
	envp = NULL;
	path_iteri = NULL;
	ft_lst_to_tab(&envp, envp_head);
	if (boolean_if_buildin(head->cmd) == 1)
		*exit_code = buildins_dispatch(head->cmd, envp_head);
	else if (cmd_access_check(head->cmd, parsed_path) == 0)
	{
		while (*parsed_path && head->cmd[0])
		{
			size = ft_strlen(*parsed_path);
			if (ft_strncmp(*parsed_path, *(head->cmd), size) != 0)
				path_iteri = ft_strjoin(*parsed_path, *(head->cmd));
			else
				path_iteri = ft_strdup(*(head->cmd));
			execve(path_iteri, head->cmd, envp);
			(parsed_path)++;
		}
		*exit_code = 127;
	}
	ft_free_tab(envp);
}

int	execute_function(t_struct *head, char **parsed_path, t_list **envp_head, int flag)
{
	char	*path_iteri;
	int		size;
	int		exit_code;
	char	**envp;

	size = 0;
	exit_code = -1;
	if (head->child < 0)
		perror("Fork:");
	else if (!head->child)
	{
		ft_dup2_infiles(head, &exit_code);
		ft_dup2_outfiles(head, &exit_code);
		if (exit_code != 1 && head->cmd)
			ft_execute_cmd(head, &exit_code, parsed_path, envp_head);
		if (flag == 1)//when forking and execve fails
		{
			ft_free_list(*envp_head);
			ft_free_tab(parsed_path);
			ft_free_sc(head);
			ft_exit(exit_code, NULL);
		}
	}
	return (exit_code);
}

int	buildins_execution(t_struct **elements, char **parsed_path, t_list **envp)
{
	t_struct	*copy;
	int			exit_code;
	int			tmp_stdin;
	int			tmp_stdout;

	exit_code = -1;
	tmp_stdin = dup(STDIN_FILENO);
	tmp_stdout = dup(STDOUT_FILENO);
	copy = *elements;
	exit_code = execute_function(copy, parsed_path, envp, 0);
	if (dup2(tmp_stdin, STDIN_FILENO) < 0)
		perror("tmp_stdin:");
	close(tmp_stdin);
	if (dup2(tmp_stdout, STDOUT_FILENO) < 0)
		perror("tmp_stdout:");
	close(tmp_stdout);
	return (exit_code);
}

int	ft_fork(t_struct **elements, char **parsed_path, t_list **envp)
{
	int			pipefds[2];
	t_struct	*copy;
	int			exit_code;

	exit_code = -1;
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
		exit_code = execute_function(copy, parsed_path, envp, 1);
		if (copy->fds[1] != 1)
			close(copy->fds[1]);
		if (copy->fds[0] != 0)
			close(copy->fds[0]);
		copy = copy->next;
	}
	copy = *elements;
	while (copy)
	{
		waitpid(copy->child, &(copy->wstatus), 0);
		copy = copy->next;
	}
	exit_code = (0xff00 & structure_last(*elements)->wstatus) >> 8;
	return (exit_code);
}

int	execute(t_struct **elements, char **parsed_path, t_list **envp)
{
	t_struct	*copy;
	int			exit_code;

	exit_code = -1;
	set_infiles_outfiles_cmds(elements);
	copy = *elements;
	if (structure_size(*elements) == 1 && boolean_if_buildin(copy->cmd) == 1)
		exit_code = buildins_execution(elements, parsed_path, envp);
	else
		exit_code = ft_fork(elements, parsed_path, envp);
	return (exit_code);
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
