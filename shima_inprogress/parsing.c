
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

int ft_iswspace(char c)
{
	if (c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v')
		return (1);
	return (0);
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

int set_infiles(char *str, char ***infiles)
{
	int i;
	int j;
	int k;
	int start;
	char **tab;
	int tab_size;

	k = 0;
	tab = ft_split(str, ' ');
	tab_size = count(str, '<');
	*infiles = (char **)malloc(sizeof(char *) * tab_size + 1);
	if (tab_size == 0)
		return (0);
	while(tab[i])
	{
		j = 0;
		while(tab[i][j])
		{
			while(tab[i][j] && tab[i][j] != '<')
				j++;
			while (tab[i][j] && tab[i][j] == '<' && tab[i][j + 1])//whatever comes after is an infile until the end of the word or whitespace or '<' or'>'
			{
				j++;
				start = j;
				while(tab[i][j] && tab[i][j] != '<' && tab[i][j] != '|' && tab[i][j] != '>')
					j++;
				(*infiles)[k] = ft_strdup_range(tab[i], start, j);
				k++;
			}
			if (tab[i][j] && tab[i][j] == '<'  && !tab[i][j + 1])// next tab has the name of the file
			{
				start = 0;
				i++;
				j = 0;
				while(tab[i][j] && tab[i][j] != '<' && tab[i][j] != '|' && tab[i][j] != '>')
					j++;
				(*infiles)[k] = ft_strdup_range(tab[i], start, j);
				k++;
			}
			else
				j++;
		}
		i++;
	}
	return (0);
}

//between the first infile and the first cmd it could be nothing (whichever comes first)
//the rest of the time cmds are bounds by |


int set_commands(char *str, char ***infiles, char ***pipex_cmds)
{
	int i;
	char **tab;

	i = 0;
	tab = remove_space(str, '<');
	if (tab[i] != '<')//the string starts with a cmd so all other cmds are bound by '|'
	{

	}
	else//the string start with the infile : find the first infile the first command after it could come with no '|'
	{

	}

}
int parsing(char *str, char ***infiles, char **outfile, char ***pipex_cmds)
{
	int i;
	int start;

	i = 0;
	start = 0;
	set_infiles(str, infiles);
	while(str[i])
	{
		if (str[i] == '>')
		{
			i++;
			while(str[i] == ' ')
				i++;
			start = i;
			while(str[i] != ' ')
				i++;
			*outfile = malloc(sizeof(char) * (i - start + 1) + 1);
			*outfile = ft_strdup_range(str, start, i);
		}
		i++;
	}
	set_commands(char *str, char ***infiles, char ***pipex_cmds);
	return (start + 1);
}


int main(int ac, char **av)
{
	char **infiles;
	char *outfile;
	char **pipex_cmds;
	int  i;
	int j;
	int k;

	i = 0;
	j = 0;
	k = 0;
	infiles = NULL;
	outfile = NULL;
	pipex_cmds = NULL;
	// while(str[i])
	// {
	// 	i = i + parsing(&str[i], &infiles, &outfile, &pipex_cmds);
	// 	while(infile[j])
	// 	{
	// 		exit_code = pipex(infile[j], &outfile, &pipex_cmds);
	// 		j++;
	// 	}


	parsing(av[1], &infiles, &outfile, &pipex_cmds);
	printf("-------the final list of inflies----------\n");
	while (infiles[k])
	{
		printf("%s\n", infiles[k]);
		(k)++;
	}
	printf("the outfile is %s\n", outfile);

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

