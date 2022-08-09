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

int number_of_here_doc(char *str)
{
	int i;
	int count;

	count = 0;
	i = 0;
	while(str[i])
	{
		if (str[i] == '<' && str[i + 1] == '<')
			count++;
		i++;
	}
	return (count);
}

char **check_for_here_doc(char *str, int **loc)
{
	int i;
	int start;
	int loc_pipe;
	char **stop;
	int size;
	int k;

	i = 0;
	loc_pipe = 0;
	k = 0;
	size = number_of_here_doc(str);
	if (size == 0)
		return (0);
	stop = malloc(sizeof(char *) * (size + 1));
	*loc = malloc(sizeof(int *) * size);
	while(str[i])
	{
		if (str[i] == '<' && str[i + 1] == '<')
		{
			i = i + 2;
			while(str[i] && str[i] == ' ')
				i++;
			start = i;
			i++;
			while (str[i] && str[i] != ' ')
				i++;
			stop[k] = ft_strjoin(ft_strdup_range(str, start, i), "\n");
			*loc[k] = loc_pipe;
			k++;
		}
		if (str[i] == '|')
			loc_pipe++;
		i++;
	}
	return (stop);
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
	execute(&elements, parsed_path, envp, str);
	exit_code = (0xff00 & sc_lstlast(elements)->wstatus) >> 8;
	printf("the exit code is %d\n", exit_code);
	return (ft_free_lst(elements), glob_free(parsed_path), exit_code);
}
/*
1. here doc
2. variable expansion
3. look for build in functions
4.
*/
