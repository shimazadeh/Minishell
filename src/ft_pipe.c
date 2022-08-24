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

void	initialize_sc(t_struct **elements, char *str)
{
	t_struct	*copy;
	int			i;
	char		**tab;

	i = 0;
	// dprintf(2,"str before split%s\n", str);
	tab = ft_split_custom(str, '|', 0);
	while (tab[i])
	{
		// dprintf(2,"str intialization %s\n", tab[i]);
		copy = ft_alloc(sizeof(t_struct));
		copy->str = ft_strdup(tab[i]);
		copy->cmd = NULL;
		copy->infiles = NULL;
		copy->outfiles = NULL;
		copy->tag = 0;
		copy->fds[0] = STDIN_FILENO;//new
		copy->fds[1] = STDOUT_FILENO;//new
		copy->wstatus = 0;//new
		copy->child = 0;
		copy->next = NULL;
		structure_add_back(elements, copy);
		i++;
		copy = copy->next;
	}
	ft_free_tab(tab);
	return ;
}

char	**extract_env_paths(char *find, t_list **envp_head)
{
	char	**paths;
	char	*temp;
	char	**tab_temp;
	int		k;

	k = 0;
	temp = NULL;
	paths = NULL;
	tab_temp = NULL;
	find_env_var(find, envp_head, &temp);
	if (temp)
	{
		tab_temp = ft_split(temp, ':');
		ft_free(temp);
		paths = ft_alloc(sizeof(char *) * (ft_strlen_tab(tab_temp) + 2));//the extra space is for adding pwd at the end
		while (tab_temp[k])
		{
			if (tab_temp[k][ft_strlen(tab_temp[k]) - 1] != '/')
				paths[k] = ft_strjoin(tab_temp[k], "/");
			else
				paths[k] = ft_strdup(tab_temp[k]);
			k++;
		}
	}
	temp = ft_getcwd();
	if (temp[ft_strlen(temp) - 1] != '/')
		paths[k] = ft_strjoin(temp, "/");
	else
		paths[k] = ft_strdup(temp);
	k++;
	paths[k] = NULL;
	ft_free(temp);
	ft_free_tab(tab_temp);
	return (paths);
}

int	ft_pipe(char *str, t_list **envp_head, int last_exit_code)
{
	char		**parsed_path;
	t_struct	*elements;
	int			exit_code;
	char		**herdoc_files;

	if (!str)
		return (0);
	elements = NULL;
	exit_code = -1;
	parsed_path = extract_env_paths("PATH", envp_head);
	variable_expansion(&str, envp_head, last_exit_code);
	initialize_sc(&elements, str);
	herdoc_files = ft_here_doc(str, &elements, envp_head, last_exit_code);
	exit_code = execute(&elements, parsed_path, envp_head);
	ft_free_sc(elements);
	ft_free_tab(parsed_path);
	ft_unlink(herdoc_files);
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
// 	parsed_path = extract_env_paths("PATH", &envp_head);//this will change
// 	initialize_sc(&elements, str);
// 	variable_expansion(&str, &envp_head, last_exit_code);
// 	execute(&elements, parsed_path, &envp_head, str);
// 	exit_code = (0xff00 & structure_last(elements)->wstatus) >> 8;
// 	ft_free_sc(elements);
// 	ft_free_tab(parsed_path);
// 	return (exit_code);
// }
