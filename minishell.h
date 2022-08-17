/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 14:32:02 by aguillar          #+#    #+#             */
/*   Updated: 2022/08/16 20:16:17 by aguillar         ###   ########.fr       */
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

extern t_list	**alloc_lst;

# define BUFFER_SIZE 100



int		pipex(char *str, t_list **envp_head, int last_exit_code);

void	rm_outer_par(char **str_add);
int		go_to_closing_char(char *str);
void	split_submod_and_sep(char **str_add, t_list **submod_head, t_list **sep_head);
int		algorithm(char *str, t_list **envp_head, int last_exit_code);

void	tab_to_list(char **tab, t_list **lst_head);

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
char	*ft_free_list(t_list *list);

void	get_prompt(char **prompt_add, t_list **envp_head);
char	**set_prompt_elems(t_list **envp_head);
char	*get_user(t_list **envp_head);
char	*get_pos(t_list **envp_head);
char	*get_pwd(t_list **envp_head, char *user);

void	free_tab_n(char **tab, int size);
char	*ft_strstr(char *str, char *to_find);

int		ft_dprintf(int fd, const char *str, ...);
int		ft_dprint(va_list arg, char c, char **to_print_add);

char	*ft_ll_itoa_base(long long int nb, char *base);
char	*ft_ll_nbr_to_string(long long int nb, char *str, int nb_len, char *base);
int		ft_ll_nb_len(long long int nb, int base_len);

char	*ft_ull_itoa_base(unsigned long long nb, char *base);
char	*ft_ull_nbr_to_string(int nb, char *str, int nb_len, char *base);
int		ft_ull_nb_len(unsigned long long int nb, int base_len);



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

#endif