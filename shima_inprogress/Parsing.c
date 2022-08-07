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

int parse_infiles(char *str, char ***dest, char delim)
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

int check_for_first_cmd(char *str, char ***pipex_cmds, char *infiles, int *k)
{
	int i;
	int j;
	int start;

	i = 0;
	j = 0;
	start = 0;
	while(str[i])
	{
		if (str[i] == infiles[j])
		{
			while (str[i] == infiles[j])
			{
				i++;
				j++;
			}
			if (!infiles[j]) //we found the first infile
			{
				while (str[i] == ' ')
					i++;
				if (str[i] == '<' || str[i] == '>' || str[i] == '|') //there is no need
					return (0);
				else //next word is the cmd
				{
					start = i;
					i++;
					while(str[i] != ' ' && str[i] != '<' && str[i] != '>')
						i++;
					(*pipex_cmds)[0] = ft_strdup_range(str, start, i);
					(*k)++;
				}
			}
			else
				j = 0;
		}
		i++;
	}
	return (0);
}

int parse_cmds(char *str, char ***dest, char delim, char ***infiles)
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
	while(*infiles && (*infiles)[j])
	{
		check_for_first_cmd(str, dest, (*infiles)[j], &k);
		j++;
	}
	j = 0;
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
//between the first infile and the first cmd it could be nothing (whichever comes first)
//the rest of the time cmds are bounds by |

int parsing_before_pipex(char **str, int i, char ***infiles, char **outfile, char ***pipex_cmds)
{
	int j;
	int k;
	char *sub_str;

	k = i;
	j = 0;
	if ((*str)[i])
	{
		while((*str)[i])
		{
			if ((*str)[i] == '>')
			{
				i++;
				while((*str)[i] == ' ')
					i++;
				j = i;
				while((*str)[i] != ' ' && (*str)[i] != '|' && (*str)[i] != '<' && (*str)[i] != '>')
					i++;
				*outfile = ft_strdup_range(*str, j, i);
				break ;
			}
			i++;
		}
		sub_str = ft_substr(*str, k, i);
		printf("the sub str is %s\n", sub_str);
		if (sub_str)
		{
			parse_infiles(sub_str, infiles, '<');
			parse_cmds(sub_str, pipex_cmds, '|', infiles);
		}
		free(sub_str);
	}
	return (i);
}


int main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)envp;
	char **infiles;
	char *outfile;
	char **pipex_cmds;
	int  i;
	// int j;
	int k;
	char *str;
	// int exit_code;

	i = 0;
	str = av[1];
	while(str[i])
	{
		infiles = NULL;
		outfile = NULL;
		pipex_cmds = NULL;
		// j = 0;
		k = 0;
		i = parsing_before_pipex(&str, i, &infiles, &outfile, &pipex_cmds);
		printf("the list of inflies----------\n");
		while (infiles && infiles[k])
		{
			printf("%s\n", infiles[k]);
			(k)++;
		}
		printf("the outfile is-----------\n %s\n", outfile);
		printf("the final list of CMDS----------\n");
		k = 0;
		while (pipex_cmds && pipex_cmds[k])
		{
			printf("%s\n", pipex_cmds[k]);
			(k)++;
		}
		printf("------------------------------------------------------------------------------------------\n");
		// while(infiles[j])
		// {
		// 	printf("execution for infile: %s\n", infiles[j]);
		// 	exit_code = pipex(infiles[j], outfile, &pipex_cmds, envp);
		// 	j++;
		// }
	}
	// parsing_before_pipex(&av[1], &infiles, &outfile, &pipex_cmds);
	glob_free(infiles);
	free(outfile);
	glob_free(pipex_cmds);
	return (0);
	// return (exit_code);
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
