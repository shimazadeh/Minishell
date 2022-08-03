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

int set_files(char *infile, char *outfile, int *fds)
{
	// if (ft_strncmp(infile, "here_doc", 9) == 0)
	// {
	// 	fds[0] = write_to_file(0, ft_strjoin(ag[2], "\n"), ag[1]);
	// 	fds[1] = open(ag[ac - 1], O_CREAT | O_RDWR | O_APPEND, 0644);
	// }
	// else if (!ft_strncmp(infile, "here_doc", 9) == 0)
	// {
		fds[0] = open(infile, O_RDONLY);
		fds[1] = open(outfile, O_CREAT | O_RDWR | O_APPEND, 0644);
	// }
	// else
	// 	return (-1);
	return (0);
}

int pipex(char *infile, char *outfile, char ***pipex_cmds, char **envp)
{
	int			fds[2];
	char		**parsed_path;
	t_struct	*elements;

	elements = NULL;
	if (set_files(infile, outfile, fds) == -1)
		return (-1);//return value should match the exit code of shell in these scenario

	if (file_access_check(infile, fds[0], outfile, fds[1]) == -1)
		return (-1); //same as above

	parsed_path = find_paths("PATH=", envp);

	initialize_lst(&elements, fds[0], fds[1], pipex_cmds);

	if (all_access_check(&elements, parsed_path) == 1)
	{
		return (ft_free_lst(elements), glob_free(parsed_path), -1);
	}

	execute(&elements, parsed_path, envp);
	return (ft_free_lst(elements), glob_free(parsed_path), 0);
}

//parsing: < infile cmd1 | cmd2 | cmd3 >outfile
/* 1. if there is no  | it is treated as a file
   2. if there is no < or > or >> it is treated as cmd
   3. if there is << it is heredoc
   4. if you have multiple infiles all of them are used
   5. if you have multiple outfules all of them get the result
   6. the start of the commands is either "|" or the cmd (<infile | cat | cat >outfile)*/
