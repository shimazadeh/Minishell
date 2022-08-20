/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithm.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 14:27:04 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/20 04:04:07 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	str_is_only_spaces(char *str)
{
	int	i;

	i = 0;
	if (!str)
		ft_exit(EXIT_FAILURE, NULL);
	while (str[i])
	{
		if (str[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}

int	go_to_closing_par(char *str)
{
	int		count;
	int		i;

	count = 0;
	i = 0;
	if (!str)
		ft_exit(EXIT_FAILURE, NULL);
	count = 1;
	i = 1;
	while (str[i])
	{
		if (str[i] == ')')
			count--;
		if (str[i] == '(')
			count++;
		if (!count && str[i] == ')')
			return (i);
		i++;
	}
	return (0);
}

void	rm_outer_par(char **str_add)
{
	int		flag;
	int		i;
	int		j;
	char	*str;
	char	*tmp;

	flag = 0;
	i = 0;
	j = 0;
	str = NULL;
	tmp = NULL;
	if (!str_add || !*str_add)
		ft_exit(EXIT_FAILURE, NULL);
	flag = 1;
	str = *str_add;
	while ((str[i] == ' ' || str[i] == '(') && flag)
	{
		if (str[i] == ' ')
			i++;
		if (str[i] == '(')
		{
			j = go_to_closing_par(&str[i]);
			if (j && str_is_only_spaces(&str[j + 1]))
			{
				i++;
				while (j > 0 && str[j - 1] == ' ')
					j--;
				str[j] = '\0';
			}
			else
				flag = 0;
		}
	}
	tmp = ft_strdup(&str[i]);
	if (!tmp)
		ft_exit(EXIT_FAILURE, NULL);
	*str_add = tmp;
	ft_free(str);
}

int	go_to_closing_char(char *str)
{
	char	c;
	int		i;

	c = 0;
	i = 0;
	if (!str)
		ft_exit(EXIT_FAILURE, NULL);
	c = *str;
	i = 1;
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
	char	*sep;
	char	*str;
	char	*submod;
	t_list	*tmp_node;

	count = 0;
	i = 0;
	j = 0;
	sep = NULL;
	str = NULL;
	submod = NULL;
	tmp_node = NULL;
	if (!str_add || !*str_add || !submod_head || !sep_head)
		ft_exit(EXIT_FAILURE, NULL);
	rm_outer_par(str_add);
	str = *str_add;
	while (str[i])
	{
		if (str[i] == '\"' || str[i] == '\'')
			i += go_to_closing_char(&str[i]);
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
				sep = ft_strdup("2");
				if (!sep)
					ft_exit(EXIT_FAILURE, NULL);
			}
			else if (ft_strncmp(&str[i], " && ", 4) == 0)
			{
				sep = ft_strdup("1");
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

// exit buildin ? shima ?
// char and wc mix

int main(int ac, char **av, char **envp)
{
	(void)av;
	int		last_exit_code;
	char	*input;
	char	*prompt;
	t_list	*envp_head;

	last_exit_code = 0;
	input = NULL;
	prompt = NULL;
	prompt = NULL;
	if (ac != 1)
	{
		ft_dprintf(2, "Error\nInvalid number of argument!\n");
		ft_exit(EXIT_FAILURE, NULL);
	}
	tab_to_list(envp, &envp_head);
	while (1)
	{
		get_prompt(&prompt, &envp_head);
		input = readline(prompt);
		if (input && *input)
		{
			if (even_par_nbr(input))
			{
				add_history(input);
				last_exit_code = algorithm(ft_strdup(input), &envp_head, last_exit_code);
			}
			else
				ft_dprintf(2, "Error\nInput contains odd number of parentheses!\n");
		}
		ft_free(input);
		ft_free(prompt);
	}
	rl_clear_history();
	ft_free_list(envp_head);
	ft_exit(EXIT_SUCCESS, NULL);
}

int	algorithm(char *str, t_list **envp_head, int last_exit_code)
{
	int 	i;
	int 	j;
	int		k;
	int		pipex_ret;
	int		ret;
	char	*tmp_str;
	t_list	*sep;
	t_list	*submod;
 	t_list	*sep_head;
 	t_list	*submod_head;

	i = 0;
	j = 0;
	k = 0;
	pipex_ret = 0;
	ret = 0;
	tmp_str = NULL;
	sep = NULL;
	submod = NULL;
	sep_head = NULL;
	submod_head = NULL;
	if (!str)
		ft_exit(EXIT_FAILURE, NULL);
	split_submod_and_sep(&str, &submod_head, &sep_head);
	if (!sep_head)
	{
		while (str[i])
		{
			if (str[i] == '\"' || str[i] == '\'')
			{
				k = go_to_closing_char(&str[i]);
				i += k;
				j += k;
			}
			if (str[i] != '(' && str[i] != ')')
				j++;
			i++;
		}
		tmp_str = str;
		str = ft_alloc(sizeof(char) * (j + 1));
		if (!str)
			ft_exit(EXIT_FAILURE, NULL);
		i = 0;
		j = 0;
		while (tmp_str[i])
		{
			if (tmp_str[i] == '\"' || tmp_str[i] == '\'')
			{
				k = go_to_closing_char(&tmp_str[i]);
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
		pipex_ret = pipex(str, envp_head, last_exit_code);
		ft_free(str);
		return (pipex_ret);
	}
	sep = sep_head;
	submod = submod_head;
	while (sep)
	{
		ret = algorithm(ft_strdup((char *)submod->content), envp_head, last_exit_code);
		if (submod_head != submod)
			sep = sep->next;
		submod = submod->next;
		if (sep && *((char *)(sep->content)) == '1' && ret)
		{
			while (sep && *((char *)(sep->content)) == '1')
			{
				submod = submod->next;
				sep = sep->next;
			}
		}
		else if (sep && *((char *)(sep->content)) == '2' && !ret)
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
