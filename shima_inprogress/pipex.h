/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shabibol <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 17:21:35 by shabibol          #+#    #+#             */
/*   Updated: 2022/07/02 17:21:39 by shabibol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stddef.h>
# include <stdlib.h>
# include "get_next_line.h"
# include "Libft/libft.h"

typedef struct s_struct
{
	char				*str;
	char				**infiles;
	char				**cmd;
	char				**outfiles;
	int					fds[2];
	int					tag;//tells you if the last infile is here_doc (1) or actual file (0)
	int					wstatus;
	pid_t				child;
	struct s_struct		*next;

}t_struct;

void		sc_lstadd_back(t_struct **lst, t_struct *new);
void		sc_lstadd_front(t_struct **lst, t_struct *new);
t_struct	*sc_lstlast(t_struct	*lst);
int			sc_lstsize(t_struct *lst);

char		**ft_free(char **dst, int i);


void		initialize_lst(t_struct **tab, char *str);

char 		*ft_strdup_range(char *str, int start, int end);
int 		parse(char *str, t_struct *node);
int 		set_infiles_outfiles_cmds(t_struct **elements);
int 		find_last_infile_type(char *str);



void		execute(t_struct **elements, char **parsed_path, char **envp, char *str);
void		execute_function(t_struct *head, char **parsed_path, char **envp);
char		*file_access_check(char **files, int flag);

char		**find_paths(char *find, char **str);
int			access_check(char **cmd, char **parsed_path);
int			all_access_check(t_struct **tab, char **parsed_path);
char		**parsing(char *find, char **str);

char 		**check_for_here_doc(char *str, int **loc);
int 		number_of_here_doc(char *str);
int			write_to_file(int fd1, char *stop, char	*file_name);
void		handle_here_doc(char *str, t_struct **elements);
void		fancy_name_generator(int size, char ***file_names);



char		**glob_free(char **dst);
void		ft_free_lst(t_struct *lst);

#endif
