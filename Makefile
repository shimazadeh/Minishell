# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: shabibol <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/07/02 17:41:06 by shabibol          #+#    #+#              #
#    Updated: 2022/08/03 13:12:37 by shabibol         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS = algorithm.c algorithm_utils.c alloc_free.c alloc_free_utils.c buildins.c buildins_utils.c ft_bzero.c ft_dprintf.c ft_exit.c ft_exit_utils.c ft_ll_itoa_base.c ft_lstadd_back.c ft_lstlast.c ft_lstnew.c ft_lstsize.c ft_putstr_fd.c ft_split.c ft_split_custom.c ft_strncmp.c ft_strndup.c ft_ull_itoa_base.c get_next_line_bonus.c get_next_line_utils_bonus.c here_doc.c Libft_Utils_lst.c Parsing.c Pipex.c Pipex_Utils_PartI.c Pipex_Utils_PartII.c prompt.c prompt_utils.c ft_strdup.c Variable_expansion.c ft_itoa.c

OBJS = $(SRCS:%.c=%.o)

FLAGS = -Wall -Wextra -Wextra -g3

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
