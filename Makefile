# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/31 18:17:31 by marcnava          #+#    #+#              #
#    Updated: 2025/09/01 00:54:44 by marcnava         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= cub3D

CC			:= gcc
CFLAGS		:= -Wall -Wextra #-Werror
DFLAGS		:= -g3 -fsanitize=address
DEBUG		?= 0
ifeq ($(DEBUG),1)
CFLAGS		+= $(DFLAGS)
endif
RM			:= rm -rf

MAKE		+= --no-print-directory --warn-undefined-variables

SRCPATH		:= src
OBJPATH		:= build
INCPATH		:= includes
LIBPATH		:= libs

LIBFT		:= $(LIBPATH)/libft
LIBMLX		:= $(LIBPATH)/minilibx

INCLUDES	:= -I$(INCPATH) -I$(LIBFT)/includes -I$(LIBMLX)/include

MLX			:= $(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm

SRCS		:= $(SRCPATH)/cub3d.c

OBJS		:= $(SRCS:$(SRCPATH)/%.c=$(OBJPATH)/%.o)

all:		$(NAME)

$(NAME):	libft libmlx $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT)/libft.a $(MLX) -o $(NAME) $(INCLUDES)

$(OBJPATH)/%.o:	$(SRCPATH)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	$(RM) $(OBJPATH)
	$(MAKE) -C $(LIBFT) clean
	-$(MAKE) -C $(LIBMLX)/build clean

fclean:		clean
	$(RM) $(NAME)
	$(RM) $(LIBPATH)

re:			fclean all

.PHONY:		all clean fclean re

libs:		libs/libft/Makefile libs/minilibx/CMakeLists.txt

libs/libft/Makefile:
	mkdir -p $(LIBPATH)
	git clone https://github.com/marcnava-42cursus/libft $(LIBFT)

libs/minilibx/CMakeLists.txt:
	mkdir -p $(LIBPATH)
	git clone https://github.com/codam-coding-college/MLX42.git $(LIBMLX)

libmlx:
	cmake $(LIBMLX) -B $(LIBMLX)/build && $(MAKE) -s -C $(LIBMLX)/build -j4

libft:
	$(MAKE) --no-print-directory -s -C $(LIBFT)

.PHONY:		libs libft libmlx