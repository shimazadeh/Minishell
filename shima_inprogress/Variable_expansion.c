
#include "pipex.h"

int	variable_expansion(char **str, t_list **envp_head)
{
	int i;
	int	k;

	i = 0;
	while(str[i])
	{
		if (str[i] == '\"')
		{
			k = go_to_closing_char(&str[i]);
			if (k)
			{
				str[i] = ' ';
				i += k;
				str[i] = ' ';
			}
		}
		if (str[i] == '\'')
		{
			k = go_to_closing_char(&str[i]);
			if (k)
			{
				str[i] = ' ';
				i += k;
				str[i] = ' ';
			}
		}
		else if (str[i] == '$' && str[i + 1] != ' ')
		{
			i++;
			j = i;
			while(str[i] != ' ')
				i++;
			tmp = ft_strdup_range(str, j, i);
			tmp2 = ft_strjoin(tmp, "=");
			find = look_up(tmp2, envp_head)
			free(tmp);
			free(tmp2);

				tmp = ft_strdup_range(str, 0, j);
				tmp2 = ft_strjoin(tmp, find);
				free(tmp);
				tmp = ft_strdup_range(str, i, ft_strlen(str));
				new_str2 = ft_strjoin(tmp, tmp2);
				*str = new_str2;
				//need to replace the str
				i = 0;
		}
		i++;
	}
}

char	*look_up(char *to_find, t_list **envp)
{
	t_list *node;
	char *result;
	int  size;

	size = ft_strlen(to_find);
	node = *envp;
	while(node)
	{
		if (!ft_strncmp(to_find, (char *)node->content, size))
		{
			result = ft_strdup_range((char *)node->content , size, ft_strlen((char *)node->content));
			return (result);
		}
		node = node->next;
	}
	return (NULL);
}
