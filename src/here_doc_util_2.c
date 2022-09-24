/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_util_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 20:29:14 by shabibol          #+#    #+#             */
/*   Updated: 2022/09/23 16:16:53 by aguillar         ###   ########.fr       */
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
	{
		if (str[i] == '\'' || str[i] == '\"')
			i = i + go_to_closing_char(&str[i]) + 1;
		else
			i++;
	}
	return (i);
}

char	*ft_readline(void)
{
	char	*gnl;
	char	*tmp;

	gnl = readline("> ");
	if (gnl)
	{
		tmp = ft_strdup(gnl);
		free(gnl);
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

int	write_to_file(t_ft_here_doc *var, int i, t_list **envp, int last_exit_code)
{
	t_wtf_vars	v[1];

	init_wtf_vars(v, var->stop[i], var->file_names[i]);
	catch_signals(2);
	if (v->fd1 < 0)
		return (ft_dprintf(2, "Error: here_doc creation failure!\n"), -1);
	v->gnl = ft_readline();
	while (!(g_var->sig_flag) && v->gnl \
		&& ft_strncmp(v->gnl, v->stop_new, ft_strlen(v->stop_new) + 1))
	{
		if (var->exp_flags[i] == 0)
			v->gnl = variable_expansion_hd(&(v->gnl), envp, last_exit_code);
		if (write(v->fd1, v->gnl, ft_strlen(v->gnl)) < 0)
			return (perror("write:"), -1);
		ft_free(v->gnl);
		v->gnl = ft_readline();
	}
	if (!v->gnl && !(g_var->sig_flag))
		ft_dprintf(1, EOF_MSG, var->stop[i]);
	close(v->fd1);
	dup2(v->stdin, 0);
	ft_free(v->gnl);
	ft_free(var->stop[i]);
	ft_free(v->stop_new);
	return (v->fd1);
}
