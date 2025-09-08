# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/31 18:17:31 by marcnava          #+#    #+#              #
#    Updated: 2025/09/06 17:45:54 by ivmirand         ###   ########.fr        #
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

SRCS		:= $(SRCPATH)/cub3d.c \
			   $(SRCPATH)/parser/parser_main.c \
			   $(SRCPATH)/parser/parser_validation.c \
			   $(SRCPATH)/parser/parse_textures.c \
			   $(SRCPATH)/parser/parse_map.c \
			   $(SRCPATH)/parser/utils.c \
			   $(SRCPATH)/parser/colors/parse_colors.c \
			   $(SRCPATH)/parser/colors/color_validation.c \
			   $(SRCPATH)/parser/colors/rgb_parsing.c \
			   $(SRCPATH)/parser/colors/value_extraction.c \
			   $(SRCPATH)/parser/utils/string_utils.c \
			   $(SRCPATH)/parser/utils/file_utils.c \
			   $(SRCPATH)/parser/utils/validation_utils.c \
			   $(SRCPATH)/parser/utils/memory_utils.c \
			   $(SRCPATH)/parser/utils/debug_utils.c \
			   $(SRCPATH)/render/window.c \
			   $(SRCPATH)/render/map_2d.c \
			   $(SRCPATH)/movement.c

OBJS		:= $(SRCS:%.c=$(OBJPATH)/%.o)

all:		$(NAME)

$(NAME):	libft libmlx $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT)/libft.a $(MLX) -o $(NAME) $(INCLUDES)

$(OBJPATH)/%.o:	%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	$(RM) $(OBJPATH)
	$(MAKE) -C $(LIBFT) clean
	-$(MAKE) -C $(LIBMLX)/build clean

fclean:		clean
	$(RM) $(NAME)
	#$(RM) $(LIBPATH)

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
