/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 14:32:02 by aguillar          #+#    #+#             */
/*   Updated: 2022/07/24 17:27:38 by aguillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H


# include <readline/readline.h>
# include <readline/history.h>

int     rm_outer_par(char **str_add);
int     split_submod_and_sep(char **str_add, t_list **submod_head, t_list **sep_head);
int     algorithm(char *str, char **envp, int last_exit_code);

char	*get_prompt(char **envp);
char	**set_prompt_elems(char **envp);
char	*get_user(char **envp);
char	*get_pos(char **envp);
char	*get_pwd(char **envp, char *user);
void	free_tab_n(char **tab, int size);
char	*ft_strstr(char *str, char *to_find);

int     pipex(char *str, char **envp, int last_exit_code);

char	**ft_split(const char *str, char c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strndup(char *s1, int j);

char	*get_next_line(int fd);
char	*get_next_line_2(int fd, char **tab, t_list *first, t_list *last);
t_list	*ft_expand_list(t_list *first, t_list *last, char *str, int size);
char	*ft_join_contents(t_list *first);
char	*ft_free_list(t_list *list);

char	*ft_strchr(const char *s, int c);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *s);
void	ft_putstr_fd(char *str, int fd);
void	ft_bzero(void *s, size_t n);

void	ft_lstadd_back(t_list **alst, t_list *new);
t_list	*ft_lstnew(void *content);
t_list	*ft_lstlast(t_list *lst);

#endif