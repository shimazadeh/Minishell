/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 14:32:02 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/20 03:22:52 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>

# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <errno.h>
# include <dirent.h>
# include <stdarg.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}				t_list;

extern t_list	*alloc_lst;

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

}				t_struct;

# define BUFFER_SIZE 100


int		pipex(char *str, t_list **envp_head, int last_exit_code);

int		str_is_only_spaces(char *str);
int		go_to_closing_par(char *str);
void	rm_outer_par(char **str_add);
int		go_to_closing_char(char *str);
void	split_submod_and_sep(char **str_add, t_list **submod_head, t_list **sep_head);
int		algorithm(char *str, t_list **envp_head, int last_exit_code);

void	tab_to_list(char **tab, t_list **lst_head);
int		even_par_nbr(char *str);

void	*ft_alloc(int size);
int		ft_free(void *to_free);

t_list	*ft_lstnew_regular(void *content);

int		buildins_dispatch(char **av, t_list **envp_head);
int		cd(char *dir, t_list **envp_head);
int		echo(char **av);
int		unset(char **vars, t_list **envp_head);
int		export(char **vars, t_list **envp_head);
int		env(t_list **envp_head);
int		pwd(void);

t_list	*old_var(char *var, t_list **envp_head);
void	print_sorted_list(t_list **envp_head);
int		print_lowest_ascii(t_list *node, char *tab);
int		ft_list_remove_node(t_list **lst_head, t_list *node);
int		contains_invalid_char(char *str, char *id, int j);
void	print_tab_nl(char **tab, int nl);
void	find_env_var(char *var_name, t_list **envp_head, char **var_exp);
void	mask_prev_compo(char *mask, char *curpath, int i);
char	*mask_result_str(char *mask, char *curpath);

void	ft_exit(int exit_code, char *error_msg);

void	ft_free_list_regular(t_list *list);

char	*get_next_line(int fd);
char	*get_next_line_2(int fd, char **tab, t_list *first, t_list *last);
t_list	*ft_expand_list(t_list *first, t_list *last, char *str, int size);
char	*ft_join_contents(t_list *first);
void	ft_free_list(t_list *list);

void	get_prompt(char **prompt_add, t_list **envp_head);
char	**set_prompt_elems(t_list **envp_head);
char	*get_user(t_list **envp_head);
char	*get_pos(t_list **envp_head);
char	*get_pwd(t_list **envp_head, char *user);

void	free_tab_n(char **tab, int size);
char	*ft_strstr(char *str, char *to_find);

int		ft_dprintf(int fd, const char *str, ...);
void	ft_dprint(va_list arg, char c, char **to_print_add);

char	*ft_ll_itoa_base(long long int nb, char *base);
char	*ft_ll_nbr_to_string(long long int nb, char *str, int nb_len, char *base);
int		ft_ll_nb_len(long long int nb, int base_len);

char	*ft_ull_itoa_base(unsigned long long nb, char *base);
char	*ft_ull_nbr_to_string(unsigned long long int nb, char *str, int nb_len, char *base);
int		ft_ull_nb_len(unsigned long long int nb, int base_len);

void	handle_wildcards(char ***av_tab_add);
void	expand_wc_node(t_list *node);
void	get_sublist(t_list **sublist, char *path);
char	**get_file_tab(char *path);
int		read_dir_content(struct dirent **dir_content_add, DIR *stream);
t_list	*get_new_path(char *path, char **file_tab);
int		compatible_name(char *file, char *wc);
int		next_char_index(char *str, char c);
void	replace_node_by_sublist(t_list *node, t_list *sublist);

void	ft_lstadd_back(t_list **alst, t_list *new);
t_list	*ft_lstnew(void *content);
t_list	*ft_lstlast(t_list *lst);
int		ft_lstsize(t_list *lst);

char	*ft_strchr(const char *s, int c);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *s);
void	ft_putstr_fd(char *str, int fd);
void	ft_bzero(void *s, size_t n);
char	**ft_split(const char *str, char c);
void	ft_free_tab(char **tab);
int		ft_strncmp(const char *s1, const char *s2, int n);
char	*ft_strndup(char *s1, int j);
char	*ft_strdup(const char	*src);
char	*ft_itoa(int n);


///**********shima's stuff***********///

//libft modified functions
void		sc_lstadd_back(t_struct **lst, t_struct *new);
void		sc_lstadd_front(t_struct **lst, t_struct *new);
t_struct	*sc_lstlast(t_struct	*lst);
int			sc_lstsize(t_struct *lst);

//variable expansion

int	variable_expansion(char **str_add, t_list **envp_head, int last_exit_code);

//parsing
void		initialize_lst(t_struct **tab, char *str);
char		*ft_strdup_range(char *str, int start, int end);
int 		parse(char *str, t_struct *node);
int 		set_infiles_outfiles_cmds(t_struct **elements);
int 		find_last_infile_type(char *str);
int			remove_double_quotes(char **str);
char		**ft_split_custom(const char *str, char c);

//execution
int			execute(t_struct **elements, char **parsed_path, t_list **envp, char *str, int last_exit_code);
int			execute_function(t_struct *head, char **parsed_path, t_list **envp_head, int sc_size);
char		*file_access_check(char **files, int flag);
void		lst_to_tab(char ***tab_add, t_list **lst_head);

//finding paths
int			access_check(char **cmd, char **parsed_path);
int			all_access_check(t_struct **tab, char **parsed_path);
char		**parsing(char *find, t_list **envp_list);

//here__doc handling
char 		**check_for_here_doc(char *str, int **loc);
int 		number_of_here_doc(char *str);
int			write_to_file(int fd1, char *stop, char	*file_name, t_list **envp_head, int last_exit_code);
char		**handle_here_doc(char *str, t_struct **elements, t_list **envp_head, int last_exit_code);
char		**fancy_name_generator(int size);
void		ft_unlink(char **file_names);

//custom free functions
void		ft_free_sc(t_struct *lst);



void	print_tab(char **tab);
void	print_list(t_list *list);




#endif
