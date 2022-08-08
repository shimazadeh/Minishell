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

int parse(char *str, char ***dest, char delim)
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
	return (1);
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
