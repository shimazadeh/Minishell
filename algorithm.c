/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithm.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 14:27:04 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/19 17:25:19 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	rm_outer_par(char **str_add)
{
	int		i;
	int		j;
	int		count;
	char	*str;
	char	*tmp;

	tmp = NULL;
	str = *str_add;
	i = 0;
	j = ft_strlen(str) - 1;
	count = 0;
	while (str[i] == ' ' || str[i] == '(')
	{
		if (str[i] == '(')
			count++;
		i++;
	}
	while (j >= 0 && (str[j] == ' ' || str[j] == ')') && count)
	{
		if (str[i] == ')')
			count--;
		j--;
	}
	while (j >= 0 && (str[j] == ' '))
		j--;
	j++;
	str[j] = '\0';
	tmp = ft_strdup(&str[i]);
	if (!tmp)
		ft_exit(EXIT_FAILURE, NULL);
	*str_add = tmp;
	ft_free(str);
}

int	go_to_closing_char(char *str, char c)
{
	int		i;

	i = 1;
	if (!c)
		c = *str;
	while (str[i] && str[i] != c)
		i++;
	if (str[i])
		return (i);
	return (0);
}

void	split_submod_and_sep(char **str_add, t_list **submod_head,
t_list **sep_head)
{
	int		count;
	int		i;
	int		j;
	char	*submod;
	char	*sep;
	char	*str;
	t_list	*tmp_node;

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
			i += go_to_closing_char(&str[i], 0);
		else if (str[i] == '(')
			count++;
		else if (str[i] == ')')
			count--;
		if (count == 0 && (ft_strncmp(&str[i], " || ", 4) == 0 || ft_strncmp(&str[i], " && ", 4) == 0))
		{
			submod = ft_strndup(&str[j], i - j);
			if (!submod)
				ft_exit(EXIT_FAILURE, NULL);
			if (ft_strncmp(&str[i], " || ", 4) == 0)
			{
				sep = ft_strndup("2", 1);
				if (!sep)
					ft_exit(EXIT_FAILURE, NULL);
			}
			else if (ft_strncmp(&str[i], " && ", 4) == 0)
			{
				sep = ft_strndup("1", 1);
				if (!sep)
					ft_exit(EXIT_FAILURE, NULL);
			}
			tmp_node = ft_lstnew(submod);
			if (!tmp_node)
				ft_exit(EXIT_FAILURE, NULL);
			ft_lstadd_back(submod_head, tmp_node);
			tmp_node = ft_lstnew(sep);
			if (!tmp_node)
				ft_exit(EXIT_FAILURE, NULL);
			ft_lstadd_back(sep_head, tmp_node);
			i += 3;
			j = i + 1;
		}
		else if (count == 0 && !str[i + 1] && submod)
		{
			submod = ft_strndup(&str[j], i + 1 - j);
			if (!submod)
				ft_exit(EXIT_FAILURE, NULL);
			tmp_node =  ft_lstnew(submod);
			ft_lstadd_back(submod_head, tmp_node);
		}
		i++;
	}
}

t_list	*alloc_lst = NULL;

int main(int ac, char **av, char **envp)
{
	(void)av;
	t_list	*envp_head;
	char	*prompt;
	char	*input;
	int		last_exit_code;

	if (ac != 1)
	{
		ft_dprintf(2, "Error\nInvalid number of argument!\n");
		ft_exit(EXIT_FAILURE, NULL);
	}
	envp_head = NULL;
	tab_to_list(envp, &envp_head);
	prompt = NULL;
	input = NULL;
	last_exit_code = 0;
	while (1)
	{
		get_prompt(&prompt, &envp_head);
		input = readline(prompt);
	//	dprintf(2, "input is '%s'\n", input);
		if (input && *input)
		{
			if (even_par_nbr(input))
			{
				add_history(input);
				last_exit_code = algorithm(ft_strndup(input, ft_strlen(input)), &envp_head, last_exit_code);
		//		printf("%s\n", input);
			}
			else
				ft_dprintf(2, "Error\nInput contains odd number of parentheses!\n");
		}
		ft_free(input);
		ft_free(prompt);
	}
	rl_clear_history();
	ft_free(prompt);
	ft_free_list(envp_head);
	ft_exit(EXIT_SUCCESS, NULL);
}

int	algorithm(char *str, t_list **envp_head, int last_exit_code)
{
 	t_list	*submod_head;
 	t_list	*sep_head;
	int 	i;
	int 	j;
	int		k;
	int		pipex_ret;
	char	*tmp_str;
	t_list	*submod;
 	t_list	*sep;
	int		ret;

	if (!str)
		ft_exit(EXIT_FAILURE, NULL);
	//dprintf(2, "str '%s'\n", str);
	submod_head = NULL;
	sep_head = NULL;
	split_submod_and_sep(&str, &submod_head, &sep_head);
	//dprintf(2, "rm outer str '%s'\n", str);
	i = 0;
	j = 0;
	k = 0;
	pipex_ret = 0;
	tmp_str = NULL;
	if (!sep_head)
	{
		while (str[i])
		{
			if (str[i] == '\"' || str[i] == '\'')
			{
				k = go_to_closing_char(&str[i], 0);
				i += k;
				j += k;
			}
			if (str[i] != '(' && str[i] != ')')
				j++;
			i++;
		}
		tmp_str = str;
		// dprintf(2, "STR %s\n", str);
		str = ft_alloc(sizeof(char) * (j + 1));
		if (!str)
			ft_exit(EXIT_FAILURE, NULL);
		i = 0;
		j = 0;
		while (tmp_str[i])
		{
			if (tmp_str[i] == '\"' || tmp_str[i] == '\'')
			{
				k = go_to_closing_char(&tmp_str[i], 0);
				while(k)
				{
					str[j] = tmp_str[i];
					i++;
					j++;
					k--;
				}
			}
			if (tmp_str[i] && tmp_str[i] != '(' && tmp_str[i] != ')')
			{
				str[j] = tmp_str[i];
				j++;
			}
			i++;
		}
		str[j] = '\0';
		ft_free(tmp_str);
		// dprintf(2, "pipex str '%s'\n", str);
		pipex_ret = pipex(str, envp_head, last_exit_code);
		ft_free(str);
		return (pipex_ret);
	}
	submod = submod_head;
	sep = sep_head;
	ret = 0;
	while (sep)
	{
		ret = algorithm(ft_strndup((char *)submod->content, ft_strlen((char *)submod->content)), envp_head, last_exit_code);
		if (submod_head != submod)
			sep = sep->next;
		submod = submod->next;
		if (sep && *((char *)(sep->content)) == '1' && !ret)
		{
			while (sep && *((char *)(sep->content)) == '1')
			{
				submod = submod->next;
				sep = sep->next;
			}
		}
		else if (sep && *((char *)(sep->content)) == '2' && ret)
		{
			submod = submod->next;
			sep = sep->next;
		}
	}
	ft_free_list(submod_head);
	ft_free_list(sep_head);
	ft_free(str);
	return (ret);
}
/*
void	display(t_list *list)
{
	t_list *copy;

	copy = list;
	while(copy)
	{
		printf("content:-------%s-------\n", (char*)(copy->content));
		copy = copy->next;
	}
}*/