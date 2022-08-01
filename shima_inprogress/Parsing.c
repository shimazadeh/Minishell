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
#include "stdio.h"
#include "stddef.h"
#include "stdlib.h"

char	**glob_free(char **dst)
{
	int	i;

	i = 0;
	while (dst && dst[i])
		i++;
	while (i > 0)
	{
		i--;
		free((void *)dst[i]);
	}
	free (dst);
	return (NULL);
}

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

int count(char *str, char c)
{
	int i;

	i = 0;
	while (*str)
	{
		if (*str == c)
			i++;
		str++;
	}
	return (i);
}

int	search_set(char **tab, int *i, int *j, int *k, char delim, char ***dest)
{
	int start;

	start = 0;
	while(tab[*i][*j] && tab[*i][*j] != delim)
		(*j)++;
	while (tab[*i][*j] && tab[*i][*j] == delim && tab[*i][*j + 1])//whatever comes after is an infile until the end of the word or whitespace or '<' or'>'
	{
		(*j)++;
		start = *j;
		while(tab[*i][*j] && tab[*i][*j] != delim && tab[*i][*j] != '|' && tab[*i][*j] != '>')
			(*j)++;
		(*dest)[*k] = ft_strdup_range(tab[*i], start, *j);
		(*k)++;
	}
	if (tab[*i][*j] && tab[*i][*j] == delim  && !tab[*i][*j + 1])// next tab has the name of the file
	{
		start = 0;
		(*i)++;
		*j = 0;
		while(tab[*i][*j] && tab[*i][*j] != delim && tab[*i][*j] != '|' && tab[*i][*j] != '>')
			(*j)++;
		(*dest)[*k] = ft_strdup_range(tab[*i], start, *j);
		(*k)++;
	}
	return(0);
}

int parse_string(char *str, char ***dest, char delim)
{
	int i;
	int j;
	int k;
	char **tab;
	int size;

	k = 0;
	i = 0;
	j = 0;
	tab = ft_split(str, ' ');
	size = count(str, delim);
	printf("tab size is %d\n", size);
	if (size == 0)
		return (0);
	*dest = (char **)malloc(sizeof(char *) * (size + 2));
	while(tab[i])
	{
		j = 0;
		while(tab[i] && tab[i][j])
			search_set(tab, &i, &j, &k, delim, dest);
		i++;
	}
	glob_free(tab);
	(*dest)[k] = '\0';
	return (0);
}
//between the first infile and the first cmd it could be nothing (whichever comes first)
//the rest of the time cmds are bounds by |

int parsing_before_pipex(char **str, char ***infiles, char **outfile, char ***pipex_cmds)
{
	int i;
	int start;
	int j;
	char *sub_str;

	i = 0;
	start = 0;
	j = 0;
	while((*str)[i])
	{
		if ((*str)[i] == '>')
		{
			start = i;
			i++;
			while((*str)[i] == ' ')
				i++;
			j = i;
			while((*str)[i] != ' ')
				i++;
			*outfile = ft_strdup_range(*str, j, i);
			break ;
		}
		i++;
	}
	sub_str = ft_substr(*str, 0, i);
	printf("the sub str is %s\n", sub_str);
	parse_string(sub_str, infiles, '<');
	parse_string(sub_str, pipex_cmds, '|');
	// we need to change the str[start] from '>' to '<' for the next round
	(*str)[start] = '<';
	free(sub_str);
	return (start);
}


int main(int ac, char **av)
{
	char **infiles;
	char *outfile;
	char **pipex_cmds;
	int  i;
	int j;
	int k;
	char *str;

	k = 0;
	i = 0;
	j = 0;
	infiles = NULL;
	outfile = NULL;
	pipex_cmds = NULL;
	str = av[1];
	while(str[i])
	{
		i = i + parsing_before_pipex(&str[i], &infiles, &outfile, &pipex_cmds);
		printf("-------the list of inflies----------\n");
		while (infiles[k])
		{
			printf("%s\n", infiles[k]);
			(k)++;
		}
		printf("the outfile is: %s\n", outfile);
		printf("-------the final list of CMDS----------\n");
		k = 0;
		while (pipex_cmds[k])
		{
			printf("%s\n", pipex_cmds[k]);
			(k)++;
		}
		// printf("the string for next round is :%s\n", *str);
		printf("-----------------------------------------\n");
		// while(infile[j])
		// {
		// 	exit_code = pipex(infile[j], &outfile, &pipex_cmds);
		// 	j++;
		// }
	}
	// parsing_before_pipex(&av[1], &infiles, &outfile, &pipex_cmds);
	glob_free(infiles);
	free(outfile);
	glob_free(pipex_cmds);
	return (0);
//	return (exit_code);
}







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
	//"<infile <infile <infile CMD1 | CMD2 | CMD3 ...| CMDN > outfile > outfile > outfile"
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
