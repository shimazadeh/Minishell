/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shabibol <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 17:38:31 by shabibol          #+#    #+#             */
/*   Updated: 2022/06/07 17:38:33 by shabibol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	set_files(int ac, char **ag, int *fds)
{
	if (ac > 5 && ft_strncmp(ag[1], "here_doc", 9) == 0)
	{
		fds[0] = write_to_file(0, ft_strjoin(ag[2], "\n"), ag[1]);
		fds[1] = open(ag[ac - 1], O_CREAT | O_RDWR | O_APPEND, 0644);
	}
	else if (ac > 3 && !ft_strncmp(ag[1], "here_doc", 9) == 0)
	{
		fds[0] = open(ag[1], O_RDONLY);
		fds[1] = open(ag[ac - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
	}
	else
		return (-1);
	return (0);
}

int	main(int ac, char **ag, char **envp)
{
	int			fds[2];
	char		**parsed_path;
	t_struct	*elements;

	elements = NULL;
	if (set_files(ac, ag, fds) == -1)
		return (-1);
	if (file_access_check(ag[1], fds[0], ag[ac - 1], fds[1]) == -1)
		return (-1);
	parsed_path = find_paths("PATH=", envp);
	initialize_lst(&elements, fds[0], fds[1], ag);
	if (all_access_check(&elements, parsed_path) == 1)
	{
		unlink("here_doc");
		return (ft_free_lst(elements), glob_free(parsed_path), -1);
	}
	pipex(&elements, parsed_path, envp);
	unlink("here_doc");
	return (ft_free_lst(elements), glob_free(parsed_path), 0);
}

//parsing: < infile cmd1 | cmd2 | cmd3 >outfile
/* 1. if there is no  | it is treated as a file
   2. if there is no < or > or >> it is treated as cmd
   3. if there is << it is heredoc
   4. if you have multiple infiles all of them are used
   5. if you have multiple outfules all of them get the result
   6. the start of the commands is either "|" or the cmd (<infile | cat | cat >outfile)*/
