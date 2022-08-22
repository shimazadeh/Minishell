# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aguillar <aguillar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/07/02 17:41:06 by shabibol          #+#    #+#              #
#    Updated: 2022/08/22 22:17:05 by aguillar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS = algorithm.c algorithm_utils.c alloc_free.c alloc_free_utils.c buildins.c buildins_utils.c ft_bzero.c ft_dprintf.c ft_exit.c ft_exit_utils.c ft_itoa.c ft_ll_itoa_base.c ft_lstadd_back.c ft_lstlast.c ft_lstnew.c ft_lstsize.c ft_putstr_fd.c ft_split.c ft_split_custom.c ft_strchr.c ft_strdup.c ft_strjoin.c ft_strlen.c ft_strncmp.c ft_strndup.c ft_strnjoin.c ft_ull_itoa_base.c get_next_line_bonus.c handle_wildcards.c here_doc.c libft_utils_lst.c parsing.c pipex.c pipex_utils_1.c pipex_utils_2.c prompt.c prompt_utils.c variable_expansion.c

OBJS = $(SRCS:%.c=%.o)

FLAGS = -Wall -Wextra -g3

all: $(NAME)

$(NAME): $(OBJS)
	gcc $(FLAGS) $(OBJS) -L/usr/include -lreadline -o $(NAME)

%.o: %.c
	gcc $(FLAGS) -c $< -o $@

clean:
	rm -f *.o

fclean: clean
	rm -rf $(NAME)

re: fclean all
