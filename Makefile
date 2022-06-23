# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbennafl <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/23 15:05:15 by mbennafl          #+#    #+#              #
#    Updated: 2022/06/23 17:06:44 by mbennafl         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =		minishell

CC =		gcc
CFLAG =		-Wall -Wextra -Werror -lreadline

SRC_PATH =	./src
SRC_FILES =	./main.c \
			./builtin_fts.c \
			./redirections.c \
			./error.c \
			./utils.c
SRC =		$(SRC_FILES:%=$(SRC_PATH)/%)
INCL =		./include


all: $(NAME)

$(NAME):
	$(CC) $(CFLAG) -I $(INCL) $(SRC) -o $(NAME)

clean:

fclean: clean
	rm -rf $(NAME)

re: fclean all
