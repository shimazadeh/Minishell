/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithm.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 14:27:04 by aguillar          #+#    #+#             */
/*   Updated: 2022/07/24 21:25:24 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	rm_outer_par(char **str_add)
{
	int		i;
	int		j;
	char	*str;
	char	*tmp;

	tmp = NULL;
	str = *str_add;
	i = 0;
	j = ft_strlen(str) - 1;
	while (i < j && (str[i] == ' ' || str[i] == '(') && (str[j] == ' '
			|| str[j] == ')'))
	{
		if (str[j] == ')' && str[i] == '(')
		{
			str[j] = '\0';
			i++;
			j--;
		}
		while (i < j && str[i] == ' ')
			i++;
		while (i < j && str[j] == ' ')
		{
			str[j] = '\0';
			j--;
		}
	}
	if (i >= j)
		return (1);
	tmp = ft_strndup(&str[i], ft_strlen(&str[i]));
	if (!tmp)
		return (0);
	*str_add = tmp;
	free(str);
	return (1);
}

void	go_to_closing_char(char **str_add)
{
	char	c;
	int		i;
	char	*str;

	str = *str_add;
	c = *str;
	i = 0;
	while (str[i] && str[i] != c)
		i++;
	if (str[i])
		*str_add = &str[i];
}

int	split_submod_and_sep(char **str_add, t_list **submod_head,
t_list **sep_head)
{
	int	count;
	int	i;
	int j;
	char *submod;
	char *sep;
	char *str;
	char *tmp;

	i = 0;
	j = 0;
	submod = NULL;
	sep = NULL;
	count = 0;
	rm_outer_par(str_add);
	str = *str_add;
	while (str[i])
	{
		if (str[i] == '\"' || str[i] == '\'')
		{
			tmp = &str[i];
			go_to_closing_char(&tmp);
		}
		else if (str[i] == '(')
			count++;
		else if (str[i] == ')')
			count--;
		if (count == 0 && (ft_strncmp(&str[i], " || ", 4) == 0 || ft_strncmp(&str[i], " && ", 4) == 0))
		{
			submod = ft_strndup(&str[j], i - j);
			if (ft_strncmp(&str[i], " || ", 4) == 0)
				sep = ft_strndup("2", 1);
			else if (ft_strncmp(&str[i], " && ", 4) == 0)
				sep = ft_strndup("1", 1);
			ft_lstadd_back(submod_head, ft_lstnew(submod));
			ft_lstadd_back(sep_head, ft_lstnew(sep));
			i += 3;
			j = i + 1;
		}
		else if (count == 0 && !str[i + 1]&& submod)
		{
			submod = ft_strndup(&str[j], i + 1 - j);
			ft_lstadd_back(submod_head, ft_lstnew(submod));
		}
		i++;
	}
	return (1);
}

int main(int ac, char **av, char **envp)
{
	(void)av;
	char	*input;
	char	*prompt;
	int		exit_code;

	if (ac != 1)
	{
		ft_putstr_fd("Error\nInvalid number of argument!\n", 2);
		return (1);
	}
	exit_code = 0;
	prompt = get_prompt(envp);
	input = readline(prompt);
	exit_code = algorithm(ft_strndup(input, ft_strlen(input)), envp, exit_code);
	if (exit_code == -1)
		exit(EXIT_FAILURE);
	add_history(input);
	while (input)
	{
		input = readline(prompt);
		exit_code = algorithm(ft_strndup(input, ft_strlen(input)), envp, last_exc);
		if (exit_code == -1)
		exit(EXIT_FAILURE);
	}
	rl_clear_history();
	free(prompt);
	exit(EXIT_SUCCESS);
}

int	algorithm(char *str, char **envp, int exit_code)
{
 	t_list	*submod;
 	t_list	*sep;
	int		ret;
 	t_list	*submod_head;
 	t_list	*sep_head;
	int		boolean;
	char	**pipex_av;
	int		pipex_ac;
	int 	i;
	int 	j;
	int		pipex_ret;

	i = 0;
	j = 0;
	pipex_ac = 0;
	pipex_av = NULL;
	pipex_ret = 0;
	submod_head = NULL;
	sep_head = NULL;
	if (!split_submod_and_sep(&str, &submod_head, &sep_head))
		return (-1);
	if (!sep_head)
	{
		while (str[i] == ' ' || str[i] == '(')
			i++;
		j = ft_strlen(str);
		while (j > 0 && (str[j] == ' ' ||  str[j] == ')'))
			j--;
		if (str[j])
			str[j] = '\0';
		pipex_ret = pipex(&str[i], envp, exit_code);
		free(str);
		return (pipex_ret);
	}
	submod = submod_head;
	sep = sep_head;
	ret = 0;
	while (sep)
	{
		ret = algorithm(ft_strndup(submod->content, ft_strlen(submod->content)), envp, exit_code);
		if (ret == -1)
		{
			free(str);
			ft_free_list(submod_head);
			ft_free_list(sep_head);
			return (-1);
		}
		else if (ret == 1)
			boolean = 1;
		else if (ret == 0)
			boolean = 0;
		if (submod_head != submod)
			sep = sep->next;
		submod = submod->next;
		if (sep && sep->content[0] == '1')
		{
			if (!boolean)
			{
				while (sep && sep->content[0] == '1')
				{
					submod = submod->next;
					sep = sep->next;
				}
			}
		}
		else if (sep && sep->content[0] == '2')
		{
			if (boolean == 1)
			{
				submod = submod->next;
				sep = sep->next;
			}
		}
	}
	ft_free_list(submod_head);
	ft_free_list(sep_head);
	free(str);
	return (boolean);
}
