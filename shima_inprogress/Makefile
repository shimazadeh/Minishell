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

NAME = pipex

LIBFT = ./Libft/libft.a

SRCS = Parsing.c Pipex.c Pipex_Utils_PartI.c Pipex_Utils_PartII.c Libft_Utils_lst.c get_next_line_utils.c get_next_line.c

SRCS_BONUS = Pipex_Bonus.c Pipex_Utils_PartI.c Pipex_Utils_PartII.c Libft_Utils_lst.c get_next_line_utils.c get_next_line.c

OBJS = $(SRCS:%.c=%.o)

OBJS_BONUS = $(SRCS_BONUS:%.c=%.o)

FLAGS = -g3

all: $(NAME)

$(NAME): $(OBJS)
	$(MAKE) -C ./Libft
	gcc $(FLAGS) $(OBJS) -o $(NAME) $(LIBFT)

%.o: %.c
	gcc $(FLAGS) -c $< -o $@

bonus: $(OBJS_BONUS)
	$(MAKE) -C ./Libft
	gcc $(FLAGS) $(OBJS_BONUS) -o $(NAME) $(LIBFT)

clean:
	rm -f *.o
	$(MAKE) clean -C ./Libft

fclean: clean
	rm -rf $(NAME)
	$(MAKE) fclean -C ./Libft

re: fclean all
