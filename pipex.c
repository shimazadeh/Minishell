/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 17:38:31 by shabibol          #+#    #+#             */
/*   Updated: 2022/08/20 22:59:29 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	initialize_lst(t_struct **elements, char *str)
{
	t_struct	*copy;
	int			i;
	char		**tab;

	i = 0;
	tab = ft_split_custom(str, '|');
	while (tab[i])
	{
		copy = ft_alloc(sizeof(t_struct));
		copy->str = ft_strdup(tab[i]);
		copy->cmd = NULL;
		copy->tag = 0;
		copy->infiles = NULL;
		copy->outfiles = NULL;
		copy->fds[0] = 0;
		copy->fds[1] = 1;
		copy->next = NULL;
		copy->child = 0;
		// copy->wstatus = 0;
		sc_lstadd_back(elements, copy);
		i++;
		copy = copy->next;
	}
	ft_free_tab(tab);
	return ;
}

// int handle_quotes(char **str_add)
// {
// 	int		i;
// 	char	*str;

// 	i = 0;
// 	str = *str_add;
// 	while (str[i])
// 	{
// 		if (str[i] == '\"' || str[i] == '\'')
// 		{
// 			k = go_to_closing_char(&str[i]);
// 			if (k)
// 			{
// 				str[i] = ' ';
// 				i += k;
// 				str[i] = ' ';
// 			}
// 		}
// 		i++;
// 	}
// 	*str_add = str;
// 	return (0);
// }

int	pipex(char *str, t_list **envp_head, int last_exit_code)
{
	char		**parsed_path;
	t_struct	*elements;
	int			exit_code;

	if(!str)
		return (0);
	elements = NULL;
	exit_code = -1;
	parsed_path = parsing("PATH", envp_head);
	variable_expansion(&str, envp_head, last_exit_code);
	//check for double quotes and remove accordingly
	initialize_lst(&elements, str);
	exit_code = execute(&elements, parsed_path, envp_head, str, last_exit_code);
	ft_free_sc(elements);
	ft_free_tab(parsed_path);
	return (exit_code);
}


// int main(int ac, char **argv, char **envp)
// {
// 	char		**parsed_path;
// 	t_struct	*elements;
// 	int			exit_code;
// 	char		*str;
// 	int 		last_exit_code;
// 	(void)ac;
// 	(void)argv;
// 	t_list *envp_head;

// 	last_exit_code = 0;
// 	envp_head = NULL;
// 	tab_to_list(envp, &envp_head);
// 	str = argv[1];//gets the entire string from Adri's algo
// 	elements = NULL;
// 	parsed_path = parsing("PATH", &envp_head);//this will change
// 	initialize_lst(&elements, str);
// 	variable_expansion(&str, &envp_head, last_exit_code);
// 	execute(&elements, parsed_path, &envp_head, str);
// 	exit_code = (0xff00 & sc_lstlast(elements)->wstatus) >> 8;
// 	ft_free_sc(elements);
// 	ft_free_tab(parsed_path);
// 	return (exit_code);
// }
