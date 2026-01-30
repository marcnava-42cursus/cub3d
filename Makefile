# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/31 18:17:31 by marcnava          #+#    #+#              #
#    Updated: 2026/01/27 19:02:05 by ivmirand         ###   ########.fr        #
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
VSYNC		?= on
MLX_SWAP_INTERVAL_VALUE := 1
ifeq ($(VSYNC),off)
	MLX_SWAP_INTERVAL_VALUE := 0
else ifeq ($(VSYNC),0)
	MLX_SWAP_INTERVAL_VALUE := 0
endif
MLX_SWAP_INTERVAL_DEFINE := -DMLX_SWAP_INTERVAL=$(MLX_SWAP_INTERVAL_VALUE)
MLX_CMAKE_FLAGS := -DCMAKE_C_FLAGS=$(MLX_SWAP_INTERVAL_DEFINE)
ifeq ($(DEBUG),1)
	CFLAGS += $(DFLAGS)
else ifeq ($(DEBUG),2)
	CFLAGS += $(DFLAGS) $(SANITIZE)
else ifeq ($(DEBUG),fps)
	CFLAGS += -DDEBUG_FPS
endif
CFLAGS += $(MLX_SWAP_INTERVAL_DEFINE)
RM			:=	rm -rf

MAKEFLAGS	+=	--no-print-directory

SRCPATH		:=	src
OBJPATH		:=	build
OBJPATH_BONUS	:=	build_bonus
INCPATH		:=	includes
LIBPATH		:=	libs

LIBFT		:=	$(LIBPATH)/libft
LIBMLX		:=	$(LIBPATH)/minilibx
MLX_SWAP_STAMP := $(LIBMLX)/build/.mlx_swap_interval

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
				$(SRCPATH)/parser/utils/link_utils.c \
				$(SRCPATH)/parser/utils/validation_utils.c \
				$(SRCPATH)/parser/utils/memory_utils.c \
				$(SRCPATH)/parser/utils/debug_utils.c \
				$(SRCPATH)/parser/utils/debug_text_utils.c \
				$(SRCPATH)/textures/texture_loader.c \
				$(SRCPATH)/render/background.c \
				$(SRCPATH)/render/double_buffer.c \
				$(SRCPATH)/render/raycast.c \
				$(SRCPATH)/render/rayhit.c \
				$(SRCPATH)/render/texture_mapping.c \
				$(SRCPATH)/render/texture_atlas.c \
				$(SRCPATH)/render/texture_atlas_utils.c \
				$(SRCPATH)/render/walls.c \
				$(SRCPATH)/render/window.c \
				$(SRCPATH)/render/utils.c \
				$(SRCPATH)/logic/movement.c \
				$(SRCPATH)/logic/movement_init.c \
				$(SRCPATH)/logic/input.c \
				$(SRCPATH)/logic/timing.c \
				$(SRCPATH)/logic/collision.c \
				$(SRCPATH)/logic/move.c \
				$(SRCPATH)/logic/rotation.c \
				$(SRCPATH)/logic/fps_overlay.c \
				$(SRCPATH)/_bonus/logic/menu/config_modal_crosshair_bonus.c \
				$(SRCPATH)/render/gameplay_window.c 

# Bonus sources - replace standard parser files with bonus versions
SRCS_BONUS	:=	$(SRCPATH)/_bonus/cub3d_bonus.c \
				$(SRCPATH)/_bonus/parser/core/parser_init_bonus.c \
				$(SRCPATH)/_bonus/parser/core/parser_orchestrator_bonus.c \
				$(SRCPATH)/_bonus/parser/link_utils_bonus.c \
				$(SRCPATH)/_bonus/parser/floor_manager_bonus.c \
				$(SRCPATH)/_bonus/parser/core/parser_finder_bonus.c \
				$(SRCPATH)/_bonus/parser/map/map_validation_bonus.c \
				$(SRCPATH)/_bonus/parser/textures/texture_parser_bonus.c \
				$(SRCPATH)/_bonus/texture_mapping_bonus.c \
				$(SRCPATH)/_bonus/logic/collision_bonus.c \
				$(SRCPATH)/_bonus/logic/collision_alias_bonus.c \
				$(SRCPATH)/parser/map/map_parser.c \
				$(SRCPATH)/parser/colors/parse_colors.c \
				$(SRCPATH)/parser/colors/color_validation.c \
				$(SRCPATH)/parser/colors/rgb_parsing.c \
				$(SRCPATH)/parser/colors/value_extraction.c \
				$(SRCPATH)/parser/textures/texture_extraction.c \
				$(SRCPATH)/parser/utils/string_utils.c \
				$(SRCPATH)/parser/utils/file_utils.c \
				$(SRCPATH)/parser/utils/link_utils.c \
				$(SRCPATH)/parser/utils/validation_utils.c \
				$(SRCPATH)/_bonus/parser/utils/memory_utils_bonus.c \
				$(SRCPATH)/parser/utils/debug_utils.c \
				$(SRCPATH)/parser/utils/debug_text_utils.c \
				$(SRCPATH)/textures/texture_loader.c \
				$(SRCPATH)/render/texture_atlas.c \
				$(SRCPATH)/render/texture_atlas_utils.c \
				$(SRCPATH)/render/player.c \
				$(SRCPATH)/render/raycast.c \
				$(SRCPATH)/render/rayhit.c \
				$(SRCPATH)/render/background.c \
				$(SRCPATH)/render/scanline_fill.c \
				$(SRCPATH)/render/utils.c \
				$(SRCPATH)/_bonus/render/bresenham_clipped.c \
				$(SRCPATH)/_bonus/render/double_buffer_bonus.c \
				$(SRCPATH)/_bonus/render/minimap.c \
				$(SRCPATH)/_bonus/render/minimap_player_vision.c \
				$(SRCPATH)/_bonus/render/minimap_tile.c \
				$(SRCPATH)/_bonus/render/minimap_utils.c \
				$(SRCPATH)/_bonus/render/outlines.c \
				$(SRCPATH)/_bonus/render/window_bonus.c \
				$(SRCPATH)/_bonus/render/map_2d_bonus.c \
				$(SRCPATH)/_bonus/logic/movement_bonus.c \
				$(SRCPATH)/_bonus/logic/input_bonus.c \
				$(SRCPATH)/_bonus/logic/controller_bonus.c \
				$(SRCPATH)/_bonus/logic/controller_bonus_state.c \
				$(SRCPATH)/_bonus/logic/controller_bonus_actions.c \
				$(SRCPATH)/_bonus/logic/controller_bonus_menu.c \
				$(SRCPATH)/_bonus/logic/controller_bonus_bindings.c \
				$(SRCPATH)/logic/timing.c \
				$(SRCPATH)/logic/move.c \
				$(SRCPATH)/logic/rotation.c \
				$(SRCPATH)/_bonus/render/gameplay_window_bonus.c \
				$(SRCPATH)/_bonus/render/walls_bonus.c \
				$(SRCPATH)/_bonus/render/floors.c \
				$(SRCPATH)/_bonus/render/ceilings.c \
				$(SRCPATH)/_bonus/render/orb.c \
				$(SRCPATH)/_bonus/render/fog.c \
				$(SRCPATH)/_bonus/render/world_map_bonus.c \
				$(SRCPATH)/_bonus/logic/inventory.c \
				$(SRCPATH)/_bonus/logic/menu/config_modal_draw_base_bonus.c \
				$(SRCPATH)/_bonus/logic/menu/config_modal_draw_rect_bonus.c \
				$(SRCPATH)/_bonus/logic/menu/config_modal_border_bonus.c \
				$(SRCPATH)/_bonus/logic/menu/config_modal_icons_bonus.c \
				$(SRCPATH)/_bonus/logic/menu/config_modal_layout_calc_bonus.c \
				$(SRCPATH)/_bonus/logic/menu/config_modal_layout_draw_bonus.c \
				$(SRCPATH)/_bonus/logic/menu/config_modal_labels_bonus.c \
				$(SRCPATH)/_bonus/logic/menu/config_modal_cards_bonus.c \
				$(SRCPATH)/_bonus/logic/menu/config_modal_hide_bonus.c \
				$(SRCPATH)/_bonus/logic/menu/config_modal_settings_draw_bonus.c \
				$(SRCPATH)/_bonus/logic/menu/config_modal_controls_draw_bonus.c \
				$(SRCPATH)/_bonus/logic/menu/config_modal_visibility_bonus.c \
				$(SRCPATH)/_bonus/logic/menu/config_modal_events_bonus.c \
				$(SRCPATH)/_bonus/logic/menu/config_modal_options_values_bonus.c \
				$(SRCPATH)/_bonus/logic/menu/config_modal_options_math_bonus.c \
				$(SRCPATH)/_bonus/logic/menu/config_modal_options_actions_bonus.c \
				$(SRCPATH)/_bonus/logic/menu/config_modal_options_mouse_bonus.c \
				$(SRCPATH)/_bonus/logic/menu/config_modal_options_init_bonus.c \
				$(SRCPATH)/_bonus/logic/menu/config_modal_fps_bonus.c \
				$(SRCPATH)/_bonus/logic/menu/config_modal_crosshair_bonus.c \
				$(SRCPATH)/_bonus/logic/menu/config_modal_controls_state_bonus.c \
				$(SRCPATH)/_bonus/logic/menu/config_modal_controls_rebind_bonus.c \
				$(SRCPATH)/_bonus/logic/menu/config_modal_controls_query_bonus.c \
				$(SRCPATH)/_bonus/logic/state_swapper.c \
				$(SRCPATH)/_bonus/logic/animations/orb_projectile_state_bonus.c \
				$(SRCPATH)/_bonus/logic/animations/orb_projectile_spawn_bonus.c \
				$(SRCPATH)/_bonus/logic/animations/orb_projectile_update_bonus.c \
				$(SRCPATH)/_bonus/animation/anim.c \
				$(SRCPATH)/_bonus/animation/player_left_hand_anims.c \
				$(SRCPATH)/_bonus/animation/player_right_hand_anims.c \
				$(SRCPATH)/_bonus/animation/player_anim.c

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
	@current_interval="$(MLX_SWAP_INTERVAL_VALUE)"; \
	stored_interval=""; \
	if [ -f "$(MLX_SWAP_STAMP)" ]; then stored_interval=$$(cat "$(MLX_SWAP_STAMP)"); fi; \
	if [ -f "$(LIBMLX)/build/libmlx42.a" ] && \
	   [ "$(LIBMLX)/CMakeLists.txt" -ot "$(LIBMLX)/build/libmlx42.a" ] && \
	   [ "$$stored_interval" = "$$current_interval" ]; then \
		echo "$(GREEN)✓ Nothing to be done for minilibx$(RESET)"; \
	else \
		echo "$(YELLOW)Compiling minilibx...$(RESET)"; \
		if cmake $(LIBMLX) -B $(LIBMLX)/build $(MLX_CMAKE_FLAGS) > /dev/null 2>&1 && \
		   $(MAKE) -s -C $(LIBMLX)/build -j4 > /dev/null 2>&1; then \
			echo "$(GREEN)✓ minilibx compiled$(RESET)"; \
			echo "$$current_interval" > "$(MLX_SWAP_STAMP)"; \
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
