
typedef struct s_cmd_list
{
	char			*cmd_str;
	int				cmd_val;
	s_cmd			*next;
	s_cmd			*prev;
}				c_cmd_list;

/////////////////////////////////////////////////////////////////

typedef struct s_var_list
{
	char				*var_content;
	struct s_var_list	*next;
}				t_var_list;

typedef struct	s_glob
{
	struct s_cmd_list	*last;
	struct s_var_list	*head;
	char				**envp;
}				t_glob;

t_glob	glob[1];

glob->s_cmd_list;
glob->envp;

/// pipex gets the s_glob and can access s_var_list: look for the variable if doesnt exist you add back
//parsing:
/* - "":  if odd treat it as string literal
			if even ignore and take it out + expand of variables (look for variable blah blah)
	- '': same as double quotes but dont expand the variable if even

redirections:  for infile if you have multiple redirection file it will execute on all
				for outfile you create all the files and the output goes to all

heredocs: << could be multiple: so you have to look for all of them bitch
		  the name of the temp file is random every time its not the same: could GNL the urandom file for 5 character maybe?

>> : if you have this for outfile you need to append to the outfile
// your number of arguments could be from 1 to anything you need to figure out what to do with it
//the structure of the arguments will have redirection for files , pipes for commands.... you need to change the argument parsing of your pipex


//"<infile <infile <infile CMD1 | CMD2 | CMD3 ...| CMDN > outfile > outfile > outfile"
//"<infile <infile <infile | CMD1 | CMD2 | CMD3 ....| CMDN >>outfile >>outfile1 >outfile2"
