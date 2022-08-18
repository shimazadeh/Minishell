/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithm_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 19:02:30 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/18 18:19:47 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	tab_to_list(char **tab, t_list **lst_head)
{
	int		i;
	t_list	*new;
	char	*str;

	i = 0;
	if (!lst_head)
		ft_exit(EXIT_FAILURE, NULL);
	if (!tab || !tab[0])
		return ;
	// dprintf(2, "we are inside the tab_list\n");
	while (tab[i])
	{
		str = ft_strndup(tab[i], ft_strlen(tab[i]));
		if (!str)
			ft_exit(EXIT_FAILURE, NULL);
		new = ft_lstnew(str);
		if (!new)
			ft_exit(EXIT_FAILURE, NULL);
		ft_lstadd_back(lst_head, new);
		// dprintf(2, "the content is %s, cont_add is %p, the node add is %p\n", (char *)new->content, new->content, new);
		i++;
	}
}

int	even_par_nbr(char *str)
{
	int	i;
	int	par;

	i = 0;
	par = 0;
	while (str[i])
	{
		if (str[i] == '(')
			par++;
		if (str[i] == ')')
			par--;
		i++;
	}
	if (par)
		return (0);
	return (1);
}
