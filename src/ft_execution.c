/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 21:17:18 by shabibol          #+#    #+#             */
/*   Updated: 2022/09/23 17:40:32 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_function(t_struct *head, char **path, t_list **envp_head, int flag)
{
	int		exit_code;

	exit_code = -1;
	if (head->child < 0)
		perror("Fork:");
	else if (!head->child)
	{
		ft_dup2_infiles(head, &exit_code);
		ft_dup2_outfiles(head, &exit_code);
		if (head->next && head->next->fds[0])
			close(head->next->fds[0]);
		if (exit_code != 1 && head->cmd)
			ft_execute_cmd(head, &exit_code, path, envp_head);
		if (flag == 1)
		{
			ft_free_list(*envp_head);
			ft_free_tab(path);
			ft_free_sc(head);
			ft_exit(exit_code, NULL, NULL);
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

int	ft_waitpid(t_struct **elements)
{
	t_struct	*copy;
	int			exit_code;

	exit_code = -1;
	copy = *elements;
	while (copy)
	{
		if (waitpid(copy->child, &(copy->wstatus), 0) != -1 \
		&& WIFEXITED(copy->wstatus))
			exit_code = WEXITSTATUS(copy->wstatus);
		else if (g_var->sig_flag == 1)
		{
			kill(copy->child, SIGQUIT);
			exit_code = 130;
		}
		else if (WCOREDUMP(copy->wstatus))
		{
			ft_dprintf(2, "segmentation fault (core dump)\n");
			exit_code = 139;
		}
		copy = copy->next;
	}
	return (exit_code);
}

int	ft_fork(t_struct **elements, char **parsed_path, t_list **envp)
{
	int			pipefds[2];
	t_struct	*copy;
	int			exit_code;

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
		catch_signals(0);
		exit_code = execute_function(copy, parsed_path, envp, 1);
		if (copy->fds[1] != 1)
			close(copy->fds[1]);
		if (copy->fds[0] != 0)
			close(copy->fds[0]);
		copy = copy->next;
	}
	exit_code = ft_waitpid(elements);
	return (exit_code);
}

int	execute(t_struct **elements, char **parsed_path, t_list **envp)
{
	t_struct	*copy;
	int			exit_code;

	exit_code = -1;
	if (set_infiles_outfiles_cmds(elements, envp) < 0)
		return (2);
	copy = *elements;
	if (g_var->sig_flag == 0)
	{
		if (struct_size(*elements) == 1 && boolean_if_buildin(copy->cmd) == 1)
			exit_code = buildins_execution(elements, parsed_path, envp);
		else
			exit_code = ft_fork(elements, parsed_path, envp);
	}
	else if (g_var->sig_flag == 1)
	{
		exit_code = 130;
		g_var->sig_flag = 0;
	}
	return (exit_code);
}
