/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shabibol <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/28 12:33:23 by shabibol          #+#    #+#             */
/*   Updated: 2022/07/28 12:33:25 by shabibol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char *ft_strdup_range(char *str, int start, int end)
{
	char *dst;
	int i;
	int k;

	k = 0;
	i = start;
	dst = malloc(sizeof(char) *ft_strlen(str) + 1);
	while (str[i] && i < end)
	{
		dst[k] = str[i];
		k++;
		i++;
	}
	dst[k] = '\0';
	return (dst);
}

int number_of_delim(char *str, char delim, int flag)//if flag == 1 it also counts >> as one
{
	int i;
	int count;

	count = 0;
	i = 0;
	while(str[i])
	{
		if (flag == 0 && str[i] == delim && str[i + 1] != delim)
			count++;
		else if (flag == 0 && str[i] == delim && str[i + 1] == delim)//skip cuz we aint care
			i++;
		else if (flag == 1 && str[i] == delim && str[i + 1] == delim)
		{
			count++;
			i++;
		}
		else if (flag == 1 && str[i] == delim && str[i + 1] != delim)
			count++;
		i++;
	}
	return (count);
}

int save_the_next_word(char **str, int i, char **dest, int to_clean)
{
	int start;

	while((*str)[i] && (*str)[i] == ' ')
		i++;
	start = i;
	while((*str)[i] && (*str)[i] != ' ' && (*str)[i] != '<' && (*str)[i] != '>')
		i++;
	*dest = ft_strdup_range(*str, start, i);
	start--;
	while(to_clean < i)
	{
		(*str)[to_clean] = ' ';
		to_clean++;
	}
	return (i);
}

int parse(char *str, t_struct *node)
{
	t_struct	*copy;
	int			infile_size;
	int			outfile_size;
	int			i;
	int			k_i;
	int			k_o;

	copy = node;
	i = 0;
	k_i = 0;
	k_o = 0;
	infile_size = number_of_delim(str, '<', 0);
	outfile_size = number_of_delim(str, '>', 1);
	if(infile_size)
		copy->infiles = (char **) malloc(sizeof(char *) * (infile_size + 1));
	if(outfile_size)
		copy->outfiles = (char **) malloc(sizeof(char *) * (outfile_size + 1));
	// printf("the str is %s\n", copy->str);
	while(str[i])
	{
		if (infile_size > 0 && str[i] == '<' && str[i + 1] != '<')//we reached the infile
		{
			i = save_the_next_word(&str, i + 1, &copy->infiles[k_i], i);
			k_i++;

		}
		else if (str[i] == '<' && str[i + 1] == '<')//we reached the heredoc replace by space and skip
		{
			str[i] = ' ';
			str[i + 1] = ' ';
			i = i + 2;
			while(str[i] == ' ')
				i++;
			while(str[i] != ' ' && str[i] != '<' && str[i] != '>' && str[i] != '|')
			{
				str[i] = ' ';
				i++;
			}
		}
		else if (str[i] == '>' && str[i + 1] != '>')//we reached the >outfile
		{
			i = save_the_next_word(&str, i + 1, &copy->outfiles[k_o], i);
			k_o++;
		}
		else if (str[i] == '>' && str[i + 1] == '>')//we reached the >>outfile
		{
			i = save_the_next_word(&str, i + 2, &copy->outfiles[k_o], i);
			k_o++;
		}
		else
			i++;
	}
	if (infile_size)
		copy->infiles[k_i] = '\0';
	if (outfile_size)
		copy->outfiles[k_o] = '\0';
	copy->cmd = ft_split(str, ' ');//whatever is left in the string is cmd
	// printf("the cmd is %s\n", copy->cmd[0]);
	// printf("the str after is %s\n", str);
	//expand on variables when you encounter $, maybe before spliting
	return (0);
}

int set_infiles_outfiles_cmds(t_struct **elements)
{
	int i;
	t_struct *copy;

	i = 0;
	copy = *elements;
	while(copy)
	{
		parse(copy->str, copy);
		copy = copy->next;
	}
	return (0);
}


// int expand_variable(char *str)
// {
// 	int i;
// 	int start;
// 	char *variable;
// 	char *replacement;

// 	i = 0;
// 	while(str[i])
// 	{
// 		if (str[i] == '$')//whatever comes after is a variable
// 		{
// 			i++;
// 			start = i;
// 			while(str[i] && str[i] != ' ')
// 				i++;
// 			variable = ft_strdup_range(str, start, i);
// 			replacement = getenv(variable);
// 			if (!replacement)//replace by space
// 			{
// 				while(start < i)
// 				{
// 					str[star] = ' ';
// 					start++;
// 				}
// 			}
// 			else //replace by the variable from start- 1  to i
// 			{

// 			}
// 		}

// 	}
// }

// int replace(char *str)
// {

// }
	/*
	a. if the first char is "<"
		1. anything with < is an infile
	b. else
		1. anything between < and | is infile

	2. anything else that doesnt start with < or > is a CMD1
	3. anything that starts with > or >> is an outfile (outfiles cannot be standalone)
*/

/*
if the first character is "<", possible formats: infiles must have < in the begining otherwise its a cmd
	//"<infile < infile<infile CMD1 | CMD2 | CMD3 ...| CMDN > outfile > outfile > outfile"
	//"<infile <infile <infile | CMD1 | CMD2 | CMD3 ....| CMDN >>outfile >>outfile1 >outfile2"

if the first one is a cmd, possible formats: between "< and | can only be infiles" and infiles could come with no "<"
	//"CMD1 <infile >> outfile1 >outfile2"
	//"CMD1 <infile infile1 | CMD2 | CMD3 >outfile2"
	//"CMD1 <infile <infile1 infile2 |CMD2 |CMD3 >outfile2"
	//"CMD1 <<EOF infile1 infile2 <infile3 |CMD1 | CMD2 >outfile"

if << exist, possible formats: infiles must have < in the begining otherwise its a cmd
	//"<< EOF <infile CMD1 | CMD2 | CMD3 >outfile" -->it writes the heredoc stuff first then infile
	//"<< EOF <infile >outfile" it copies the content of here_doc and infile to outfile

ouftfiles always have > or >> if it doesnt its a cmd
*/
//<infile <infile <infile CMD1 | CMD2 | CMD3 ...| CMDN
//<    infile <    infile <     infile CMD1 | CMD2 | CMD3 ...| CMDN


//ask Adrien wtf is this <infile 3<infile1 cat | cat >outfile //any number works except negative or 1
