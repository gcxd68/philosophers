# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gdosch <gdosch@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/05 11:47:52 by gdosch            #+#    #+#              #
#    Updated: 2025/05/21 20:15:31 by gdosch           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC =			philo.c \
				getset.c \
				sim.c \
				utils.c

OBJ =			$(SRC:.c=.o)

NAME =			philo
CC =			cc
CFLAGS =		-Wall -Wextra -Werror# -g3 -fsanitize=thread

all:			$(NAME)

$(NAME):		$(OBJ)
				$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

clean:
				rm -f $(OBJ)

fclean:			clean
				rm -f $(NAME)

re:				fclean all

.PHONY:			all clean fclean re
