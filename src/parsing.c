/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/28 12:33:23 by shabibol          #+#    #+#             */
/*   Updated: 2022/09/23 16:42:48 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	save_next_word(char **str_add, int i, char **dest, int to_clean)
{
	int		start;
	char	*str;

	str = *str_add;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	start = i;
	while (str[i] && !end_char(str[i], "> \t<\0"))
	{
		if (str[i] == '\'' || str[i] == '\"')
			i = supress_the_quotes(&str, i);
		else
			i++;
	}
	*dest = ft_strdup_range(str, start, i);
	*str_add = replace_with_space(str, ++to_clean, i);
	if (!*dest)
		return (ft_dprintf(1, "bash: syntax error near unexpected token\n"), -1);
	return (i);
}

int	parse_infiles(char **str_add, t_struct *head)
{
	char		*str;
	int			i;
	int			k;

	i = 0;
	k = -1;
	str = *str_add;
	while (i >= 0 && str[i])
	{
		if (str[i] == '\"' || str[i] == '\'')
			i = i + go_to_closing_char(&str[i]) + 1;
		else if (str[i] && str[i] == '<' && str[i + 1] != '<')
		{
			i = save_next_word(&str, i + 1, &head->infiles[++k], i - 1);
			head->infile_modes[k] = 0;
		}
		else
			i++;
	}
	head->infiles[++k] = '\0';
	return (*str_add = str, i);
}

int	parse_outfiles(char **str_add, t_struct *head)
{
	char		*str;
	int			i;
	int			k;

	i = -1;
	k = -1;
	str = *str_add;
	while (str[++i] && i >= 0)
	{
		if (str[i] == '\"' || str[i] == '\'')
			i = i + go_to_closing_char(&str[i]);
		else if (str[i] && str[i] == '>' && str[i + 1] != '>')
		{
			i = save_next_word(&str, i + 1, &head->outfiles[++k], i - 1) - 1;
			head->outfile_modes[k] = 1;
		}
		else if (str[i] && str[i] == '>' && str[i + 1] == '>')
		{
			i = save_next_word(&str, i + 2, &head->outfiles[++k], i - 1) - 1;
			head->outfile_modes[k] = 2;
		}
	}
	head->outfiles[++k] = '\0';
	return (*str_add = str, i);
}

int	set_files(t_struct *head, int flag)
{
	int	size;

	size = 0;
	if (flag == 1)
	{
		size = number_of_delim(head->str, '<', 0);
		head->infiles = ft_alloc(sizeof(char *) * (size + 1));
		head->infile_modes = ft_alloc(sizeof(int) * size);
		if (parse_infiles(&head->str, head) < 0)
			return (-1);
	}
	if (flag == 0)
	{
		size = number_of_delim(head->str, '>', 1);
		head->outfiles = ft_alloc(sizeof(char *) * (size + 1));
		head->outfile_modes = ft_alloc(sizeof(int) * size);
		if (parse_outfiles(&head->str, head) < 0)
			return (-1);
	}
	return (0);
}

int	set_infiles_outfiles_cmds(t_struct **elements, t_list **envp)
{
	t_struct	*copy;

	copy = *elements;
	while (copy)
	{
		if (number_of_delim(copy->str, '<', 0))
		{
			if (set_files(copy, 1) < 0)
				return (-1);
		}
		if (number_of_delim(copy->str, '>', 1))
		{
			if (set_files(copy, 0) < 0)
				return (-1);
		}
		copy->cmd = ft_split_custom(copy->str, ' ', 1);
		handle_wildcards(&copy->cmd);
		if (!((*elements)->next))
			export_last_cmd(copy->cmd, envp);
		copy = copy->next;
	}
	return (0);
}
