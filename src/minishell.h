/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 14:32:02 by aguillar          #+#    #+#             */
/*   Updated: 2022/09/27 16:23:06 by aguillar         ###   ########.fr       */
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

# define EOF_MSG "bash: warning: here-document delimited by end-of-file \
(wanted `%s')\n"

# define SHLVL_ERR "bash: warning: shell level (%s) too high, resetting to 1\n"

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}				t_list;

typedef struct s_global
{
	int				sig_flag;
	t_list			*g_alloc_lst;
}				t_global;

extern t_global	*g_var;

typedef struct s_struct
{
	char				*str;
	char				**infiles;
	char				**cmd;
	char				**outfiles;
	int					*outfile_modes;
	int					*infile_modes;
	int					fds[2];
	int					tag;
	int					wstatus;
	pid_t				child;
	int					sig;
	struct s_struct		*next;

}				t_struct;

# define BUFFER_SIZE 10

///////////////////////////////////////////////////////////////////////////////

// minishell.c

typedef struct s_minishell_vars
{
	int				last_exit_code;
	char			*input;
	char			*prompt;
	t_list			*envp_head;
}				t_minishell_vars;

void		minishell_init_vars(t_minishell_vars v[1]);
void		minishell(char	**envp);
void		minishell_loop(t_minishell_vars *v, char *tmp);

// minishell_utils.c

int			even_par_nbr(char *str);
int			no_unclosed_quote(char *str);
void		handle_ws(char **str_add);
void		handle_ws_get_str_size(int *size_add, char *str);
void		handle_ws_fill_str(char *str, char *new_str);

// syntax_check.c

int			syntax_check(char **str_add);
void		handle_quotes(char **str_add, int *i_add);
char		*handle_single_quote(int i, int j, char *str, int quote);
int			is_special_char(char *str, int *ch_code_add);
int			print_syntax_error(int ch_code);

// handle_special_char.c

int			handle_special_char(char *str, int *i_add, int ch_code, \
char *par_tab);
int			handle_special_char_loop(int i, int ch_code, char *str);
int			handle_special_char_loop_check_prev(int i, \
int ch_code, char *str);

// algorithm.c

typedef struct s_aglorithm_vars
{
	int				pipex_ret;
	int				ret;
	char			*tmp_str;
	t_list			*sep;
	t_list			*submod;
	t_list			*sep_head;
	t_list			*submod_head;
}				t_aglorithm_vars;

int			algorithm(char *str, t_list **envp_head, int last_exit_code);
int			algorithm_exit_condition_1(char *str, t_list **envp_head, \
int last_exit_code, t_aglorithm_vars v[1]);
int			algorithm_exit_condition_2(char *str, t_list **envp_head, \
int last_exit_code, t_aglorithm_vars v[1]);
int			algorithm_recursive(t_list **envp_head, int last_exit_code, \
t_aglorithm_vars v[1]);

// algorithm_utils.c

int			go_to_closing_char(char *str);
void		rm_outer_par_1(char **str_add);
void		rm_outer_par_2(int *i_add, int j, int flag, char *str);
int			go_to_closing_par(char *str, char *par_tab);
int			str_is_only_spaces(char *str);

// split_submod_and_sep.c

typedef struct s_split_submod_and_sep_vars
{
	int				count;
	int				i;
	int				j;
	char			*str;
}				t_split_submod_and_sep_vars;

void		split_submod_and_sep_1(char **str_add, t_list **submod_head, \
t_list **sep_head);
void		split_submod_and_sep_2(t_list **submod_head, \
t_list **sep_head, t_split_submod_and_sep_vars v[1]);
void		split_submod_and_sep_init_vars(t_split_submod_and_sep_vars v[1]);

// garbage_collector.c

void		*ft_alloc(int size);
int			ft_free(void *to_free);
int			free_node(t_list *prev, t_list *current);

// buildins_dispatch.c

int			buildins_dispatch(char **av, t_list **envp_head);

// cd.c

typedef struct s_cd_vars
{
	int				ret;
	int				*cd_no_path_ret;
	char			*curpath;
	char			*mask;
	char			*pwd_exp;
	char			*tmp;
}				t_cd_vars;

void		cd_init_vars(t_cd_vars v[1]);
int			cd(char **dir_tab, t_list **envp_head);
int			cd_check(char **dir_tab, char **dir_add);
void		cd_path_to_curpath(char *dir, t_cd_vars *v);
int			cd_canon_and_exec(t_list **envp_head, t_cd_vars *v, char *dir);

// cd_no_arg.c

void		cd_no_arg(char **dir_add, t_list **envp_head);

// cd_hyphen.c

int			cd_hyphen(t_list **envp_head);

// cd_no_path_1.c

int			cd_no_path_check(char *dir);
void		cd_no_path(int **cd_no_path_ret_add, char *dir, \
char **curpath_add, t_list **envp_head);
void		cd_no_path_no_cdpath_exp(char *dir, char **curpath_add);

// cd_no_path_2.c

typedef struct s_cd_no_path_cdpath_exp_vars
{
	char				*curpath;
	char				*tmp;
	char				**cd_paths;
	int					*cd_no_path_ret;

}				t_cd_no_path_cdpath_exp_vars;

void		cd_no_path_cdpath_exp_init_vars(t_cd_no_path_cdpath_exp_vars v[1], \
char *cdpath_exp);
void		cd_no_path_cdpath_exp(int **cd_no_path_ret_add, \
char *cdpath_exp, char *dir, t_list **envp_head);
void		handle_valid_cdpath(t_cd_no_path_cdpath_exp_vars v[1], \
t_list **envp_head);

// cd_curpath_no_abs_path.c

void		cd_curpath_no_abs_path(char **curpath_add, char *pwd_exp);

// cd_get_canon_curpath_mask.c

typedef struct s_cd_curpath_is_dot_vars
{
	char	*prev_compo;
	char	*prev_compo_path;
}				t_cd_curpath_is_dot_vars;

int			cd_get_canon_curpath_mask(char **mask_add, char *curpath);
void		cd_init_mask(char **mask_add, char *curpath, int *i_add);
void		cd_curpath_is_dot_init_vars(t_cd_curpath_is_dot_vars v[1], \
int *j_add, int *i_add);
int			cd_curpath_is_dot(char *curpath, char *mask, int *j_add);

// cd_to_export.c

int			cd_to_export_1(char *curpath, char *pwd_exp, t_list **envp_head);
int			cd_to_export_2(char ***to_export_add, t_list **envp_head, \
char *var_name, char *var_exp);

// cd_utils.c

void		prev_compo_2dot_or_root(char *path, char *mask, int i, \
t_cd_curpath_is_dot_vars v[1]);
void		mask_prev_compo(char *mask, char *path, int i);
char		*mask_result_str(char *mask, char *path);

// echo.c

int			echo(char **av);

// echo_utils.c

void		print_tab_nl(char **tab, int nl);

// env.c

int			env(t_list **envp_head);

// export.c

int			export(char **vars, t_list **envp_head);
int			export_no_arg(t_list **envp_head);
int			export_arg(int i, int ret, char **vars, t_list **envp_head);

// export_utils.c

void		print_sorted_list(t_list **envp_head);
int			print_lowest_ascii(int i, int lowest_ascii_mask, \
t_list *node, char *tab);
void		print_export_var(char *lowest_ascii);
t_list		*old_var(char *var, t_list **envp_head);

// pwd.c

int			pwd(void);

// unset.c

typedef struct s_unset_vars
{
	t_list			*node;
	t_list			*tmp;
	int				i;
	int				ret;
}				t_unset_vars;

void		unset_init_vars(t_unset_vars v[1]);
int			unset(char **vars, t_list **envp_head);
void		unset_exec(int i, char **vars, t_list **envp_head, \
t_unset_vars v[1]);

// unset_utils.c

int			ft_list_remove_node(t_list **lst_head, t_list *node);

// buildins_utils.c

int			contains_invalid_char(char *str, char *id, int j);
int			find_env_var(char *var_name, t_list **envp_head, \
char **var_exp_add);
char		*ft_getcwd(void);

// ft_exit.c

void		ft_exit(int exit_code, char *fn_name, char *error_msg);

// prompt.c

void		get_prompt(char **prompt_add, t_list **envp_head);
char		**set_prompt_elems(t_list **envp_head);
char		*get_user(t_list **envp_head);
char		*get_pos(t_list **envp_head);
char		*get_pwd(t_list **envp_head, char *user);

// handle_widlcards.c

void		handle_wildcards(char ***av_tab_add);
void		expand_wc_node(t_list *node);
void		replace_node_by_sublist(t_list *node, t_list *sublist);

// handle_wildcards_utils.c

void		trim_extra_wc(char	*str, char **path_add);
void		get_str_size(int i, int j, int *j_add, char *str);
void		fill_str(int i, int j, char *str, char *path);
void		get_opendir_path(char *path, char **opendir_path_add);

// get_sublist.c

typedef struct s_get_sublist_vars
{
	char			*new_opendir_path;
	char			*new_path;
	char			*opendir_path;
	char			*path;
	char			*wc;
	t_list			*file_lst;
	t_list			*new_path_lst;
	t_list			**sublist;
}				t_get_sublist_vars;

void		get_sublist_init_vars(t_list **sublist, char *path, \
char *opendir_path, t_get_sublist_vars v[1]);
void		get_sublist(t_list **sublist, char *path, char *opendir_path);
void		get_sublist_recursive(t_get_sublist_vars v[1]);

// get_file_list.c

void		get_file_list(char *path, t_list **file_lst_add);
int			read_dir_content(struct dirent **dir_content_add, DIR *stream);

// get_new_path_list.c

typedef struct s_get_new_path_list_vars
{
	char			*path;
	char			*path_start;
	char			*path_end;
	char			*wc_str;
	t_list			*file_lst;
	t_list			*new_path_lst;

}				t_get_new_path_list_vars;

void		get_new_path_list_init_vars(char *path, t_list *file_lst, \
t_get_new_path_list_vars v[1]);
void		get_new_path_list(char *path, t_list *file_lst, \
t_list **new_path_lst_add);
int			fill_new_path_list(int i, int j, t_get_new_path_list_vars v[1]);

// get_new_path_list_utils.c

typedef struct s_compatible_name_vars
{
	char			*file;
	char			*wc;
}				t_compatible_name_vars;

int			next_char_index(char *str, char c);
int			compatible_name(char *file, char *wc);
int			compatible_name_init(int *i_add, int *j_add, int *k_add, \
t_compatible_name_vars v[1]);
void		compatible_name_loop(int *i_add, int *j_add, int *k_add, \
t_compatible_name_vars v[1]);

// ft_dprintf.c

typedef struct s_ft_dprint_vars
{
	char					char_var[1];
	char					*str_var;
	unsigned long long int	ulli_var;
	long long int			lli_var;
	char					*char_tmp;
	char					*to_print;
}				t_ft_dprint_vars;

void		ft_dprint_init_vars(char **to_print_add, t_ft_dprint_vars v[1]);
int			ft_dprintf(int fd, const char *str, ...);
void		ft_dprint(va_list arg, char c, char **to_print_add);

// ft_dprint_utils.c

void		ft_dprint_1(va_list arg, char c, char **to_print_add, \
t_ft_dprint_vars v[1]);
void		ft_dprint_2(va_list arg, char c, t_ft_dprint_vars v[1]);
void		ft_dprint_3(va_list arg, char c, t_ft_dprint_vars v[1]);
void		ft_dprint_4(va_list arg, char c, t_ft_dprint_vars v[1]);

// get_next_line_bonus.c

char		*get_next_line(int fd);
char		*get_next_line_2(int fd, char **tab, t_list *first, t_list *last);

// get_next_line_utils_bonus.c

t_list		*ft_expand_list(t_list *first, t_list *last, char *str, int size);
char		*ft_join_contents(t_list *first);

// ft_split_cutom.c

typedef struct s_ft_split_custom_vars
{
	char			c;
	int				flag;
	int				wc;
	const char		*str;
	char			**tab;
}				t_split_custom_vars;

typedef struct s_ft_fill_tab_custom_vars
{
	int				gtcc;
	int				i;
	int				j;
	int				k;
}				t_ft_fill_tab_custom_vars;

void		ft_split_custom_init_vars(const char *str, char c, int flag, \
t_split_custom_vars v[1]);
char		**ft_split_custom(const char *str, char c, int flag);
void		ft_fill_tab_custom_init_vars(t_ft_fill_tab_custom_vars w[1]);
int			ft_fill_tab_custom(t_split_custom_vars v[1]);

// ft_split_custom_utils.c

int			ft_wc_custom(char *str, char c);
int			ft_wl_custom(char *str, char c, int flag);
void		ft_get_wl(char *str, int *i_add, int *wl_add, int flag);
void		ft_get_tab(t_split_custom_vars v[1], \
t_ft_fill_tab_custom_vars w[1]);

// adjust_env.c

void		adjust_env(t_list **envp_head);
void		adjust_env_underscore(char *to_export[2], char *pwd, \
t_list **envp_head);
int			contains_non_digit(char *str);
int			shlvl_too_high(char *shlvl);

// libft

int			ft_atoi(const char *str);
void		ft_bzero(void *s, size_t n);
void		ft_free_list(t_list *list);
void		ft_free_list_regular(t_list *list);
void		ft_free_tab(char **tab);
void		ft_free_tab_n(char **tab, int size);
char		*ft_itoa(int n);
char		*ft_ll_itoa_base(long long int nb, char *base);
void		ft_lstadd_back(t_list **alst, t_list *new);
t_list		*ft_lstlast(t_list *lst);
t_list		*ft_lstnew(void *content);
t_list		*ft_lstnew_regular(void *content);
int			ft_lstsize(t_list *lst);
void		ft_lst_to_tab(char ***tab_add, t_list **lst_head);
void		ft_putstr_fd(char *str, int fd);
char		**ft_split(const char *str, char c);
char		*ft_strchr(const char *s, int c);
char		*ft_strdup(const char *src);
char		*ft_strdup_range(char *str, int start, int end);
char		*ft_strjoin(char const *s1, char const *s2);
size_t		ft_strlen(const char *s);
int			ft_strncmp(const char *s1, const char *s2, int n);
char		*ft_strndup(char *s1, int j);
char		*ft_strnjoin(char const *s1, char const *s2, int size);
char		*ft_strstr(char *str, char *to_find);
char		*ft_ull_itoa_base(unsigned long long nb, char *base);
void		ft_tab_to_lst(char **tab, t_list **lst_head);

// export_last_cmd.c

void		export_last_cmd(char **cmd, t_list **envp);
void		export_next_cmd(char *cmd, t_list **envp);

// print_tools.c

void		print_list(t_list *list);
void		print_tab(char **tab);

///**********shima's stuff***********///

int			ft_pipe(char *str, t_list **envp_head, int last_exit_code);

//libft modified functions
void		structure_add_back(t_struct **lst, t_struct *new);
t_struct	*structure_last(t_struct	*lst);
int			struct_size(t_struct *lst);

//variable expansion

char		*variable_expansion(char *str, t_list **envp_head, int last_ec);
char		*expand_variable(char *str, int *i, int last_ec, \
t_list **envp_head);
int			end_char(char c, char *end);
char		*create_new_str(char *str, char *to_add, int *to_break, \
int to_continue);

//parsing
void		initialize_sc(t_struct **tab, char *str);
void		assign_str_to_struct(t_struct **elements, char *str);
int			parse(char *str, t_struct *node);
int			parse_outfiles(char **str_add, t_struct *head);
int			parse_infiles(char **str_add, t_struct *head);
int			set_infiles_outfiles_cmds(t_struct **elements, t_list **envp);
int			set_files(t_struct *head, int flag);
int			find_last_infile_type(char *str);
int			save_next_word(char **str_add, int i, \
char **dest, int to_clean);
int			number_of_delim(char *str, char delim, int flag);
int			move_the_char_back(char *str);
char		*replace_with_space(char *str, int start, int end);
int			supress_the_quotes(char **str_add, int i);

//execution
int			execute(t_struct **elements, char **parsed_path, t_list **envp);
int			execute_function(t_struct *head, char **parsed_path, \
t_list **envp_head, int sc_size);
int			ft_fork(t_struct **elements, char **parsed_path, t_list **envp);
void		ft_dup2_infiles(t_struct *head, int *exit_code);
void		ft_dup2_outfiles(t_struct *head, int *exit_code);
void		ft_execute_cmd(t_struct *head, int *exit_code, \
char **parsed_path, t_list **envp_head);

// char		*file_access_check(char **files, int flag);
int			file_access_check(char **file, int *file_modes);
int			boolean_if_buildin(char **av);
char		*cmd_access_check(char **cmd, char **parsed_path, \
int *last_exit_code);
char		*cmd_check(char **path, int *ec, t_struct *head);
void		print_error(char *cmd);
int			ft_waitpid(t_struct **elements);

//finding paths
char		**extract_env_paths(char *find, t_list **envp_list);

//here_doc handling
typedef struct s_ft_here_doc
{
	char	**file_names;
	char	**stop;
	int		*exp_flags;
	int		*loc;
	char	*str;
	int		*fds;
	int		size;
}				t_ft_here_doc;

typedef struct s_wtf_vars
{
	char		*gnl;
	int			fd1;
	char		*stop_new;
	int			stdin;
}				t_wtf_vars;

void		initialize_heredoc_var(t_ft_here_doc *v);
char		**ft_here_doc(char **str, t_struct **elements, \
t_list **envp, int exit);
char		**store_heredoc_stops(char **str_add, int *flag, int size);
int			*store_heredoc_loc(char *str, int size);
char		**default_name_generator(int size);
char		**fancy_name_generator(int size);
int			number_of_here_doc(char *str);
int			set_last_infile_type(t_struct **elements, char **files, \
int *loc, int size);
int			find_last_infile_type(char *str);
void		ft_unlink(char **file_names);
int			write_to_file(t_ft_here_doc *var, int i, \
t_list **envp, int last_exit_code);
int			write_to_file_2(t_wtf_vars v[1], char *stop);
int			pass_the_next_word(char *str);
char		*variable_expansion_hd(char **str_add, \
t_list **envp_head, int last_ec);
char		*ft_readline(void);
void		init_wtf_vars(t_wtf_vars v[1], char *stop, char	*file);
int			quotes_presence(char *str);

//signals
void		sig_handler(int signum, siginfo_t *info, void *ptr);
void		sig_handler_pipe(int signum, siginfo_t *info, void *ptr);
void		sig_handler_heredoc(int signum, siginfo_t *info, void *ptr);
void		catch_signals(int flag);

//custom free functions
void		ft_free_sc(t_struct *lst);
int			ft_strlen_tab(char **tab);

#endif
