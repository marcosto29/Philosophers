# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marcos <marcos@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/08 11:55:33 by marcos            #+#    #+#              #
#    Updated: 2025/07/11 17:23:49 by marcos           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

CFLAGS = -g -Wall -Werror -Wextra

SRCS = main.c \
		input_parse.c \
		philo_create.c \
		free_memory.c \

NAME = philo

OBJS = $(SRCS:.c=.o)

%.o:%.c
	$(CC) $(CFLAGS) -c $<

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS)

clean:
	rm -rf $(OBJS)
	
fclean:
	rm -rf $(OBJS) $(NAME)