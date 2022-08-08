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

void	initialize_lst(t_struct **elements, char *str)
{
	t_struct	*copy;
	int			i;
	char		**tab;

	i = 0;
	tab = ft_split(str, '|');
	while (tab[i])
	{
		copy = malloc(sizeof(t_struct));
		copy->str = ft_strdup(tab[i]);
		copy->cmd = NULL;
		copy->infiles = NULL;
		copy->outfiles = NULL;
		copy->next = NULL;
		sc_lstadd_back(elements, copy);
		i++;
		copy = copy->next;
	}
	glob_free(tab);
	return ;
}

int main(int ac, char **argv, char **envp)
{
	char		**parsed_path;
	t_struct	*elements;
	int			exit_code;
	char		*str;
	(void)ac;
	(void)argv;

	str = argv[1];//gets the entire string from Adri's algo
	elements = NULL;
	parsed_path = parsing("PATH=", envp);
	initialize_lst(&elements, str);
	execute(&elements, parsed_path, envp);
	exit_code = (0xff00 & sc_lstlast(elements)->wstatus) >> 8;
	printf("the exit code is %d\n", exit_code);
	return (ft_free_lst(elements), glob_free(parsed_path), exit_code);
}

//parsing: < infile cmd1 | cmd2 | cmd3 >outfile
/* 1. if there is no  | it is treated as a file
   2. if there is no < or > or >> it is treated as cmd
   3. if there is << it is heredoc
   4. if you have multiple infiles all of them are used
   5. if you have multiple outfules all of them get the result
   6. the start of the commands is either "|" or the cmd (<infile | cat | cat >outfile)*/
