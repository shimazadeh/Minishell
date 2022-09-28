/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution_util_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 21:34:38 by shabibol          #+#    #+#             */
/*   Updated: 2022/09/23 17:06:37 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error(char *cmd)
{
	DIR		*stream;

	stream = opendir(&cmd[0]);
	if (access(&cmd[0], F_OK))
		ft_dprintf(2, "bash: %s: command not found\n", &cmd[0]);
	else if (access(&cmd[0], X_OK))
		ft_dprintf(2, "bash: %s: Permission denied\n", &cmd[0]);
	else if (stream && errno != ENOTDIR)
	{
		ft_dprintf(2, "bash: %s: command not found\n", &cmd[0]);
		closedir(stream);
	}
}

char	*cmd_access_check(char **cmd, char **parsed_path, int *last_exit_code)
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
			return (ft_free(path_iteri), parsed_path[i]);
		else if (access(path_iteri, F_OK) == 0 && access(path_iteri, X_OK) == 0)
		{
			stream = opendir(path_iteri);
			if (!stream && errno == ENOTDIR)
				return (path_iteri);
			else if (stream)
				closedir(stream);
		}
		ft_free(path_iteri);
	}
	print_error(cmd[0]);
	return (*last_exit_code = 127, NULL);
}

char	*cmd_check(char **path, int *ec, t_struct *head)
{
	DIR		*stream;
	char	*result;

	stream = opendir(head->cmd[0]);
	result = NULL;
	if (ft_strchr(head->cmd[0], '/'))
	{
		if (access(head->cmd[0], F_OK) == 0 && access(head->cmd[0], X_OK) == 0 \
		&& !stream)
			result = ft_strdup(head->cmd[0]);
		else
		{
			ft_dprintf(2, "bash: %s: no such file or directory\n", head->cmd[0]);
			*ec = 127;
		}
	}
	else
	{
		closedir(stream);
		result = cmd_access_check(head->cmd, path, ec);
	}
	return (result);
}
