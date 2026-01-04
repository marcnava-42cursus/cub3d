# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/31 18:17:31 by marcnava          #+#    #+#              #
#    Updated: 2026/01/04 17:29:05 by ivmirand         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= cub3D
NAME_BONUS	:= cub3D_bonus

CC			:= gcc
CFLAGS		:= -Wall -Wextra #-Werror
CFLAGS		+= -MMD -MP
DFLAGS		:= -g3
SANITIZE	:= -fsanitize=address
DEBUG		?= 0
ifeq ($(DEBUG),1)
	CFLAGS += $(DFLAGS)
else ifeq ($(DEBUG),2)
	CFLAGS += $(DFLAGS) $(SANITIZE)
endif
RM			:=	rm -rf

MAKEFLAGS	+=	--no-print-directory

SRCPATH		:=	src
OBJPATH		:=	build
OBJPATH_BONUS	:=	build_bonus
INCPATH		:=	includes
LIBPATH		:=	libs

LIBFT		:=	$(LIBPATH)/libft
LIBMLX		:=	$(LIBPATH)/minilibx

INCLUDES	:=	-I$(INCPATH) -I$(LIBFT)/includes -I$(LIBMLX)/include

MLX			:=	$(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm

SRCS		:=	$(SRCPATH)/cub3d.c \
				$(SRCPATH)/parser/core/parser_init.c \
				$(SRCPATH)/parser/core/parser_finder.c \
				$(SRCPATH)/parser/core/parser_orchestrator.c \
				$(SRCPATH)/parser/map/map_characters.c \
				$(SRCPATH)/parser/map/map_player.c \
				$(SRCPATH)/parser/map/map_validation.c \
				$(SRCPATH)/parser/map/map_parser.c \
				$(SRCPATH)/parser/colors/parse_colors.c \
				$(SRCPATH)/parser/colors/color_validation.c \
				$(SRCPATH)/parser/colors/rgb_parsing.c \
				$(SRCPATH)/parser/colors/value_extraction.c \
				$(SRCPATH)/parser/textures/texture_extraction.c \
				$(SRCPATH)/parser/textures/texture_parser.c \
				$(SRCPATH)/parser/textures/texture_setter.c \
				$(SRCPATH)/parser/utils/string_utils.c \
				$(SRCPATH)/parser/utils/file_utils.c \
				$(SRCPATH)/parser/utils/validation_utils.c \
				$(SRCPATH)/parser/utils/memory_utils.c \
				$(SRCPATH)/parser/utils/debug_utils.c \
				$(SRCPATH)/parser/utils/debug_text_utils.c \
				$(SRCPATH)/textures/texture_loader.c \
				$(SRCPATH)/render/background.c \
				$(SRCPATH)/render/bresenham.c \
				$(SRCPATH)/render/double_buffer.c \
				$(SRCPATH)/render/minimap.c \
				$(SRCPATH)/render/raycast.c \
				$(SRCPATH)/render/rayhit.c \
				$(SRCPATH)/render/texture_mapping.c \
				$(SRCPATH)/render/walls.c \
				$(SRCPATH)/render/outlines.c \
				$(SRCPATH)/render/window.c \
				$(SRCPATH)/render/utils.c \
				$(SRCPATH)/render/map_2d.c \
				$(SRCPATH)/logic/movement.c \
				$(SRCPATH)/logic/input.c \
				$(SRCPATH)/logic/timing.c \
				$(SRCPATH)/logic/collision.c \
				$(SRCPATH)/logic/move.c \
				$(SRCPATH)/logic/rotation.c \
				$(SRCPATH)/logic/debug_map.c \
				$(SRCPATH)/render/gameplay_window.c \
				$(SRCPATH)/render/player.c \
				$(SRCPATH)/render/floors.c \
				$(SRCPATH)/render/world_map.c

# Bonus sources - replace standard parser files with bonus versions
SRCS_BONUS	:=	$(SRCPATH)/cub3d.c \
				$(SRCPATH)/parser/core/parser_init.c \
				$(SRCPATH)/parser/core/parser_orchestrator.c \
				$(SRCPATH)/_bonus/parser_finder_bonus.c \
				$(SRCPATH)/_bonus/map_validation_bonus.c \
				$(SRCPATH)/_bonus/texture_parser_bonus.c \
				$(SRCPATH)/render/walls.c \
				$(SRCPATH)/render/texture_mapping.c \
				$(SRCPATH)/_bonus/collision_bonus.c \
				$(SRCPATH)/parser/map/map_parser.c \
				$(SRCPATH)/parser/colors/parse_colors.c \
				$(SRCPATH)/parser/colors/color_validation.c \
				$(SRCPATH)/parser/colors/rgb_parsing.c \
				$(SRCPATH)/parser/colors/value_extraction.c \
				$(SRCPATH)/parser/textures/texture_extraction.c \
				$(SRCPATH)/parser/utils/string_utils.c \
				$(SRCPATH)/parser/utils/file_utils.c \
				$(SRCPATH)/parser/utils/validation_utils.c \
				$(SRCPATH)/parser/utils/memory_utils.c \
				$(SRCPATH)/parser/utils/debug_utils.c \
				$(SRCPATH)/parser/utils/debug_text_utils.c \
				$(SRCPATH)/textures/texture_loader.c \
				$(SRCPATH)/render/texture_atlas.c \
				$(SRCPATH)/render/texture_atlas_utils.c \
				$(SRCPATH)/render/raycast.c \
				$(SRCPATH)/render/rayhit.c \
				$(SRCPATH)/render/background.c \
				$(SRCPATH)/render/bresenham.c \
				$(SRCPATH)/render/double_buffer.c \
				$(SRCPATH)/render/minimap.c \
				$(SRCPATH)/render/minimap_player_vision.c \
				$(SRCPATH)/render/scanline_fill.c \
				$(SRCPATH)/render/minimap_tile.c \
				$(SRCPATH)/render/minimap_utils.c \
				$(SRCPATH)/render/outlines.c \
				$(SRCPATH)/render/window.c \
				$(SRCPATH)/render/utils.c \
				$(SRCPATH)/render/map_2d.c \
				$(SRCPATH)/logic/movement.c \
				$(SRCPATH)/logic/input.c \
				$(SRCPATH)/logic/timing.c \
				$(SRCPATH)/logic/move.c \
				$(SRCPATH)/logic/rotation.c \
				$(SRCPATH)/logic/debug_map.c \
				$(SRCPATH)/render/gameplay_window.c \
				$(SRCPATH)/render/player.c \
				$(SRCPATH)/render/floors.c \
				$(SRCPATH)/render/world_map.c \
				$(SRCPATH)/_bonus/inventory.c \
				$(SRCPATH)/_bonus/state_swapper.c

OBJS		:= $(SRCS:%.c=$(OBJPATH)/%.o)
OBJS_BONUS	:= $(SRCS_BONUS:%.c=$(OBJPATH_BONUS)/%.o)
DEPS		:= $(OBJS:.o=.d)
DEPS_BONUS	:= $(OBJS_BONUS:.o=.d)

# Colors
YELLOW		:= \033[0;33m
GREEN		:= \033[0;32m
RED			:= \033[0;31m
DARK_GRAY	:= \033[0;90m
RESET		:= \033[0m
BOLD		:= \033[1m

# ASCII Art
define ASCII_ART
$(BOLD)
   ██████╗██╗   ██╗██████╗ ██████╗ ██████╗
  ██╔════╝██║   ██║██╔══██╗╚════██╗██╔══██╗
  ██║     ██║   ██║██████╔╝ █████╔╝██║  ██║
  ██║     ██║   ██║██╔══██╗ ╚═══██╗██║  ██║
  ╚██████╗╚██████╔╝██████╔╝██████╔╝██████╔╝
   ╚═════╝ ╚═════╝ ╚═════╝ ╚═════╝ ╚═════╝
$(RESET)
endef
export ASCII_ART

-include $(DEPS)
-include $(DEPS_BONUS)

.DEFAULT_GOAL := all

all:
	@echo "$$ASCII_ART"
	@$(MAKE) _compile BONUS_FLAG=""

bonus:
	@echo "$$ASCII_ART"
	@echo "$(YELLOW)$(BOLD)>>> BONUS MODE ENABLED <<<$(RESET)"
	@$(MAKE) _compile_bonus BONUS_FLAG="-D BONUS"

_compile:	libft libmlx $(OBJS)
	@if [ ! -f $(NAME) ]; then \
		echo "$(GREEN)✓ Linking $(NAME)...$(RESET)"; \
		$(CC) $(CFLAGS) $(OBJS) $(LIBFT)/libft.a $(MLX) -o $(NAME); \
		echo "$(GREEN)$(BOLD)✓ $(NAME) compiled successfully!$(RESET)\n"; \
	elif [ $(NAME) -ot $(LIBFT)/libft.a ] || [ $(NAME) -ot $(LIBMLX)/build/libmlx42.a ] || [ -n "$(shell find $(OBJPATH) -name '*.o' -newer $(NAME) 2>/dev/null)" ]; then \
		echo "$(GREEN)✓ Linking $(NAME)...$(RESET)"; \
		$(CC) $(CFLAGS) $(OBJS) $(LIBFT)/libft.a $(MLX) -o $(NAME); \
		echo "$(GREEN)$(BOLD)✓ $(NAME) compiled successfully!$(RESET)\n"; \
	else \
		echo "$(GREEN)✓ Nothing to be done for $(NAME)$(RESET)\n"; \
	fi

_compile_bonus:	libft libmlx $(OBJS_BONUS)
	@if [ ! -f $(NAME_BONUS) ]; then \
		echo "$(GREEN)✓ Linking $(NAME_BONUS)...$(RESET)"; \
		$(CC) $(CFLAGS) $(OBJS_BONUS) $(LIBFT)/libft.a $(MLX) -o $(NAME_BONUS); \
		echo "$(GREEN)$(BOLD)✓ $(NAME_BONUS) compiled successfully!$(RESET)\n"; \
	elif [ $(NAME_BONUS) -ot $(LIBFT)/libft.a ] || [ $(NAME_BONUS) -ot $(LIBMLX)/build/libmlx42.a ] || [ -n "$(shell find $(OBJPATH_BONUS) -name '*.o' -newer $(NAME_BONUS) 2>/dev/null)" ]; then \
		echo "$(GREEN)✓ Linking $(NAME_BONUS)...$(RESET)"; \
		$(CC) $(CFLAGS) $(OBJS_BONUS) $(LIBFT)/libft.a $(MLX) -o $(NAME_BONUS); \
		echo "$(GREEN)$(BOLD)✓ $(NAME_BONUS) compiled successfully!$(RESET)\n"; \
	else \
		echo "$(GREEN)✓ Nothing to be done for $(NAME_BONUS)$(RESET)\n"; \
	fi

$(OBJPATH)/%.o:	%.c
	@mkdir -p $(@D)
	@printf "$(DARK_GRAY)\rCompiling %-50s$(RESET)" "$<"
	@$(CC) $(CFLAGS) $(BONUS_FLAG) $(INCLUDES) -o $@ -c $< && \
		printf "$(GREEN)\r✓ Compiled %-50s$(RESET)\n" "$<" || \
		(printf "$(RED)\r✗ Failed compiling %-50s$(RESET)\n" "$<" && exit 1)

$(OBJPATH_BONUS)/%.o:	%.c
	@mkdir -p $(@D)
	@printf "$(DARK_GRAY)\rCompiling %-50s$(RESET)" "$<"
	@$(CC) $(CFLAGS) $(BONUS_FLAG) $(INCLUDES) -o $@ -c $< && \
		printf "$(GREEN)\r✓ Compiled %-50s$(RESET)\n" "$<" || \
		(printf "$(RED)\r✗ Failed compiling %-50s$(RESET)\n" "$<" && exit 1)

clean:
	@echo "$(YELLOW)Cleaning object files...$(RESET)"
	@$(RM) $(OBJPATH) $(OBJPATH_BONUS)
	@$(MAKE) -C $(LIBFT) clean > /dev/null 2>&1
	@-$(MAKE) -C $(LIBMLX)/build clean > /dev/null 2>&1
	@echo "$(GREEN)✓ Clean complete$(RESET)"

fclean:		clean
	@echo "$(YELLOW)Removing binaries...$(RESET)"
	@$(RM) $(NAME) $(NAME_BONUS)
	@$(RM) $(LIBPATH)
	@echo "$(GREEN)✓ Full clean complete$(RESET)"

libclean:
	@echo "$(YELLOW)Removing libraries...$(RESET)"
	@$(RM) $(LIBPATH)
	@echo "$(GREEN)✓ Libraries removed$(RESET)"

re:			fclean all

relib:		libclean libft libmlx

libs/libft/Makefile:
	@mkdir -p $(LIBPATH)
	@echo "$(YELLOW)Cloning libft...$(RESET)"
	@git clone https://github.com/marcnava-42cursus/libft $(LIBFT) > /dev/null 2>&1
	@echo "$(GREEN)✓ libft cloned$(RESET)"

libs/minilibx/CMakeLists.txt:
	@mkdir -p $(LIBPATH)
	@echo "$(YELLOW)Cloning minilibx...$(RESET)"
	@git clone https://github.com/codam-coding-college/MLX42.git $(LIBMLX) > /dev/null 2>&1
	@echo "$(GREEN)✓ minilibx cloned$(RESET)"

libmlx:	$(LIBMLX)/CMakeLists.txt
	@if [ -f "$(LIBMLX)/build/libmlx42.a" ] && [ "$(LIBMLX)/CMakeLists.txt" -ot "$(LIBMLX)/build/libmlx42.a" ]; then \
		echo "$(GREEN)✓ Nothing to be done for minilibx$(RESET)"; \
	else \
		echo "$(YELLOW)Compiling minilibx...$(RESET)"; \
		if cmake $(LIBMLX) -B $(LIBMLX)/build > /dev/null 2>&1 && \
		   $(MAKE) -s -C $(LIBMLX)/build -j4 > /dev/null 2>&1; then \
			echo "$(GREEN)✓ minilibx compiled$(RESET)"; \
		else \
			echo "$(RED)✗ Failed compiling minilibx$(RESET)"; \
			exit 1; \
		fi; \
	fi

libft:	$(LIBFT)/Makefile
	@if [ -f "$(LIBFT)/libft.a" ] && [ "$(LIBFT)/Makefile" -ot "$(LIBFT)/libft.a" ]; then \
		echo "$(GREEN)✓ Nothing to be done for libft$(RESET)"; \
	else \
		echo "$(YELLOW)Compiling libft...$(RESET)"; \
		if $(MAKE) -s -C $(LIBFT) > /dev/null 2>&1; then \
			echo "$(GREEN)✓ libft compiled$(RESET)"; \
		else \
			echo "$(RED)✗ Failed compiling libft$(RESET)"; \
			exit 1; \
		fi; \
	fi

reload:	all
	./cub3D maps/example.cub

reload_bonus:	bonus
	./cub3D_bonus maps/example.cub

.PHONY:		all bonus clean fclean libclean re relib libft libmlx reload reload_bonus
