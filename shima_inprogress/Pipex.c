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
		copy->tag = 0;
		copy->infiles = NULL;
		copy->outfiles = NULL;
		copy->fds[0] = 0;
		copy->fds[1] = 0;
		copy->next = NULL;
		sc_lstadd_back(elements, copy);
		i++;
		copy = copy->next;
	}
	glob_free(tab);
	return ;
}


// int	shimalgo(char *str, t_list **envp_head, int	last_exit_code);

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
	variable_expansion(&str, envp);
	// execute(&elements, parsed_path, envp, str);
	// exit_code = (0xff00 & sc_lstlast(elements)->wstatus) >> 8;
	ft_free_lst(elements);
	glob_free(parsed_path);
	printf("the new str is %s\n", str);
	return (exit_code);
}
/*
1. here doc
2. variable expansion
3. look for build in functions
4.
*/
