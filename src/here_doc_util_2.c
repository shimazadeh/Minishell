/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_util_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shabibol <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 20:29:14 by shabibol          #+#    #+#             */
/*   Updated: 2022/08/25 20:29:16 by shabibol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pass_the_next_word(char *str)
{
	int	i;

	i = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (str[i] && (str[i] == '\'' || str[i] == '\"'))
		return (i + go_to_closing_char(&str[i]) + 1);
	while (str[i] && str[i] != ' ' && str[i] != '\t')
		i++;
	if (str[i])
		return (i);
	return (0);
}

char	*ft_readline(void)
{
	char	*gnl;
	char	*tmp;

	gnl = readline("> ");
	if (gnl)
	{
		tmp = gnl;
		gnl = ft_strjoin(tmp, "\n");
		ft_free(tmp);
	}
	return (gnl);
}

void	init_wtf_vars(t_wtf_vars v[1], char *stop, char	*file)
{
	v->stdin = dup(0);
	v->stop_new = ft_strjoin(stop, "\n");
	v->fd1 = open(file, O_CREAT | O_RDWR, 0777);
}

int	write_to_file(char *stop, char	*file, t_list **envp, int last_exit_code)
{
	t_wtf_vars	v[1];

	init_wtf_vars(v, stop, file);
	if (v->fd1 < 0)
		return (ft_dprintf(2, "error creating here_doc\n"), -1);
	v->gnl = ft_readline();
	while (!(g_var->sig_flag) && v->gnl \
		&& ft_strncmp(v->gnl, v->stop_new, ft_strlen(v->stop_new) + 1))
	{
		v->gnl = variable_expansion_hd(&(v->gnl), envp, last_exit_code);
		if (write(v->fd1, v->gnl, ft_strlen(v->gnl)) < 0)
			return (perror("write:"), -1);
		ft_free(v->gnl);
		v->gnl = ft_readline();
	}
	return (write_to_file_2(v, stop));
}

int	write_to_file_2(t_wtf_vars v[1], char *stop)
{
	if (!v->gnl && g_var->sig_flag)
	{
		write(1, "\n", 1);
		dup2(v->stdin, 0);
	}
	else if (!v->gnl && !(g_var->sig_flag))
		ft_dprintf(1, \
			"warning: here-document delimited by end-of-file (wanted `%s')\n", \
			stop);
	return (close(v->fd1), ft_free(v->gnl), ft_free(stop), \
		ft_free(v->stop_new), v->fd1);
}
