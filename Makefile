# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/31 18:17:31 by marcnava          #+#    #+#              #
#    Updated: 2026/02/08 04:19:12 by ivmirand         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Name of the executables
NAME		:= cub3D
NAME_BONUS	:= cub3D_bonus

# Compiler and flags
CC			:= gcc
CFLAGS		:= -Wall -Wextra #-Werror
CFLAGS		+= -MMD -MP
DFLAGS		:= -g3
SANITIZE	:= -fsanitize=address
DEBUG		?= 0
VSYNC		?= off
MLX_SWAP	:= 0

# VSync configuration for minilibx
ifeq ($(VSYNC),on)
	MLX_SWAP := 1
else ifeq ($(VSYNC),1)
	MLX_SWAP := 1
endif

MLX_SWAP_INTERVAL_DEFINE := -DMLX_SWAP_INTERVAL=$(MLX_SWAP)
MLX_CMAKE_FLAGS := -DCMAKE_C_FLAGS=$(MLX_SWAP_INTERVAL_DEFINE)

# Debug flags
ifeq ($(DEBUG),1)
	CFLAGS += $(DFLAGS)
else ifeq ($(DEBUG),2)
	CFLAGS += $(DFLAGS) $(SANITIZE)
else ifeq ($(DEBUG),fps)
	CFLAGS += -DDEBUG_FPS
endif

# Add MLX swap interval define to CFLAGS for main compilation
CFLAGS		+=	$(MLX_SWAP_INTERVAL_DEFINE)
# File management
RM			:=	rm -rf
MAKEFLAGS	+=	--no-print-directory

# Directory structure
SRCPATH		:=	src
OBJPATH		:=	build
OBJPATH_B	:=	build_bonus
INCPATH		:=	includes
LIBPATH		:=	libs

# Folders
BONUS		:=	$(SRCPATH)/_bonus
PARSER		:=	$(SRCPATH)/parser
LOGIC		:=	$(SRCPATH)/logic
RENDER		:=	$(SRCPATH)/render
PARSER_B	:=	$(BONUS)/parser
LOGIC_B		:=	$(BONUS)/logic
RENDER_B	:=	$(BONUS)/render
ANIMATION	:=	$(BONUS)/animation
AUDIO		:=	$(BONUS)/audio

LIBFT		:=	$(LIBPATH)/libft
LIBMLX		:=	$(LIBPATH)/minilibx
MLX_SWAP_STAMP := $(LIBMLX)/build/.mlx_swap_interval

INCLUDES	:=	-I$(INCPATH) -I$(LIBFT)/includes -I$(LIBMLX)/include -I$(LIBPATH)

MLX			:=	$(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm

# Source files
SRCS		:=	$(SRCPATH)/cub3d.c

SRCS		+=	\
				$(PARSER)/core/parser_init.c \
				$(PARSER)/core/parser_finder.c \
				$(PARSER)/core/parser_orchestrator.c \
				$(PARSER)/map/map_characters.c \
				$(PARSER)/map/map_player.c \
				$(PARSER)/map/map_validation.c \
				$(PARSER)/map/map_parser.c \
				$(PARSER)/colors/parse_colors.c \
				$(PARSER)/colors/color_validation.c \
				$(PARSER)/colors/rgb_parsing.c \
				$(PARSER)/colors/value_extraction.c \
				$(PARSER)/textures/texture_extraction.c \
				$(PARSER)/textures/texture_parser.c \
				$(PARSER)/textures/texture_setter.c \
				$(PARSER)/utils/string_utils.c \
				$(PARSER)/utils/file_utils.c \
				$(PARSER)/utils/link_utils.c \
				$(PARSER)/utils/validation_utils.c \
				$(PARSER)/utils/memory_utils.c \
				$(PARSER)/utils/debug_utils.c \
				$(PARSER)/utils/debug_text_utils.c

SRCS		+=	\
				$(LOGIC)/movement.c \
				$(LOGIC)/movement_init.c \
				$(LOGIC)/input.c \
				$(LOGIC)/timing.c \
				$(LOGIC)/collision.c \
				$(LOGIC)/move.c \
				$(LOGIC)/rotation.c \
				$(LOGIC)/fps_overlay.c \
				$(LOGIC)/texture_loader.c

SRCS		+=	\
				$(RENDER)/background.c \
				$(RENDER)/double_buffer.c \
				$(RENDER)/raycast.c \
				$(RENDER)/rayhit.c \
				$(RENDER)/texture_mapping.c \
				$(RENDER)/texture_atlas.c \
				$(RENDER)/frame_to_image.c \
				$(RENDER)/texture_atlas_utils.c \
				$(RENDER)/walls.c \
				$(RENDER)/window.c \
				$(RENDER)/utils.c \
				$(RENDER)/gameplay_window.c 

# Bonus source files
SRCS_BONUS	:=	$(SRCPATH)/_bonus/cub3d_bonus.c

SRCS_BONUS	+=	\
				$(PARSER)/map/map_parser.c \
				$(PARSER)/colors/parse_colors.c \
				$(PARSER)/colors/color_validation.c \
				$(PARSER)/colors/rgb_parsing.c \
				$(PARSER)/colors/value_extraction.c \
				$(PARSER)/textures/texture_extraction.c \
				$(PARSER)/utils/string_utils.c \
				$(PARSER)/utils/file_utils.c \
				$(PARSER)/utils/link_utils.c \
				$(PARSER)/utils/validation_utils.c \
				$(PARSER)/utils/debug_utils.c \
				$(PARSER)/utils/debug_text_utils.c \
				$(PARSER_B)/core/parser_init_bonus.c \
				$(PARSER_B)/core/parser_orchestrator_bonus.c \
				$(PARSER_B)/link_utils_bonus.c \
				$(PARSER_B)/floor_manager_bonus.c \
				$(PARSER_B)/core/parser_finder_bonus.c \
				$(PARSER_B)/map/map_validation_bonus.c \
				$(PARSER_B)/textures/texture_parser_bonus.c \
				$(PARSER_B)/utils/memory_utils_bonus.c

SRCS_BONUS	+=	\
				$(LOGIC)/texture_loader.c \
				$(LOGIC)/timing.c \
				$(LOGIC)/move.c \
				$(LOGIC)/rotation.c \
				$(LOGIC_B)/movement_bonus.c \
				$(LOGIC_B)/input_bonus.c \
				$(LOGIC_B)/controller_bonus.c \
				$(LOGIC_B)/controller_bonus_state.c \
				$(LOGIC_B)/controller_bonus_actions.c \
				$(LOGIC_B)/controller_bonus_menu.c \
				$(LOGIC_B)/controller_bonus_bindings.c \
				$(LOGIC_B)/inventory.c \
				$(LOGIC_B)/menu/config_modal_draw_base_bonus.c \
				$(LOGIC_B)/menu/config_modal_draw_rect_bonus.c \
				$(LOGIC_B)/menu/config_modal_border_bonus.c \
				$(LOGIC_B)/menu/config_modal_icons_bonus.c \
				$(LOGIC_B)/menu/config_modal_layout_calc_bonus.c \
				$(LOGIC_B)/menu/config_modal_layout_draw_bonus.c \
				$(LOGIC_B)/menu/config_modal_labels_bonus.c \
				$(LOGIC_B)/menu/config_modal_cards_bonus.c \
				$(LOGIC_B)/menu/config_modal_hide_bonus.c \
				$(LOGIC_B)/menu/config_modal_settings_draw_bonus.c \
				$(LOGIC_B)/menu/config_modal_controls_draw_bonus.c \
				$(LOGIC_B)/menu/config_modal_visibility_bonus.c \
				$(LOGIC_B)/menu/config_modal_events_bonus.c \
				$(LOGIC_B)/menu/config_modal_options_values_bonus.c \
				$(LOGIC_B)/menu/config_modal_options_math_bonus.c \
				$(LOGIC_B)/menu/config_modal_options_actions_bonus.c \
				$(LOGIC_B)/menu/config_modal_options_mouse_bonus.c \
				$(LOGIC_B)/menu/config_modal_options_init_bonus.c \
				$(LOGIC_B)/menu/config_modal_fps_bonus.c \
				$(LOGIC_B)/menu/config_modal_crosshair_bonus.c \
				$(LOGIC_B)/menu/config_modal_controls_state_bonus.c \
				$(LOGIC_B)/menu/config_modal_controls_rebind_bonus.c \
				$(LOGIC_B)/menu/config_modal_controls_query_bonus.c \
				$(LOGIC_B)/state_swapper.c \
				$(LOGIC_B)/animations/orb_projectile_state_bonus.c \
				$(LOGIC_B)/animations/orb_projectile_ghost_bonus.c \
				$(LOGIC_B)/animations/orb_projectile_spawn_bonus.c \
				$(LOGIC_B)/animations/orb_projectile_update_bonus.c \
				$(LOGIC_B)/collision_bonus.c

SRCS_BONUS	+=	\
				$(RENDER)/texture_atlas.c \
				$(RENDER)/frame_to_image.c \
				$(RENDER)/texture_atlas_utils.c \
				$(RENDER)/raycast.c \
				$(RENDER)/rayhit.c \
				$(RENDER)/background.c \
				$(RENDER)/scanline_fill.c \
				$(RENDER)/utils.c \
				$(RENDER_B)/texture_mapping/texture_mapping_bonus.c \
				$(RENDER_B)/texture_mapping/paint_pixel_color_bonus.c \
				$(RENDER_B)/texture_mapping/paint_vertical_line_texture_bonus.c \
				$(RENDER_B)/living_block.c \
				$(RENDER_B)/player.c \
				$(RENDER_B)/bresenham_clipped.c \
				$(RENDER_B)/double_buffer_bonus.c \
				$(RENDER_B)/minimap.c \
				$(RENDER_B)/minimap_player_vision.c \
				$(RENDER_B)/minimap_player.c \
				$(RENDER_B)/minimap_walls.c \
				$(RENDER_B)/minimap_utils.c \
				$(RENDER_B)/outlines.c \
				$(RENDER_B)/window_bonus.c \
				$(RENDER_B)/gameplay_window_bonus.c \
				$(RENDER_B)/walls_bonus.c \
				$(RENDER_B)/floors_and_ceilings.c \
				$(RENDER_B)/door.c \
				$(RENDER_B)/orb.c \
				$(RENDER_B)/fog.c \
				$(RENDER_B)/absorb.c 

SRCS_BONUS	+=	\
				$(AUDIO)/orb_audio_bonus.c \
				$(AUDIO)/orb_distance_audio_bonus.c \
				$(AUDIO)/step_audio_bonus.c

SRCS_BONUS	+=	\
				$(ANIMATION)/anim.c \
				$(ANIMATION)/anim_utils.c \
				$(ANIMATION)/living_block_anims.c \
				$(ANIMATION)/absorb_anims.c \
				$(ANIMATION)/effects_anims.c \
				$(ANIMATION)/orb_anims.c \
				$(ANIMATION)/door_anims.c \
				$(ANIMATION)/door_anim_states.c \
				$(ANIMATION)/player/player_anim.c \
				$(ANIMATION)/player/player_left_hand_anims.c \
				$(ANIMATION)/player/player_right_hand_anims.c 

OBJS		:= $(SRCS:%.c=$(OBJPATH)/%.o)
OBJS_BONUS	:= $(SRCS_BONUS:%.c=$(OBJPATH_B)/%.o)
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
		printf "$(GREEN)✓ Linking $(NAME)...$(RESET)\n"; \
		$(CC) $(CFLAGS) $(OBJS) $(LIBFT)/libft.a $(MLX) -o $(NAME); \
		printf "$(GREEN)$(BOLD)✓ $(NAME) compiled successfully!$(RESET)\n\n"; \
	elif [ $(NAME) -ot $(LIBFT)/libft.a ] || [ $(NAME) -ot $(LIBMLX)/build/libmlx42.a ] || [ -n "$(shell find $(OBJPATH) -name '*.o' -newer $(NAME) 2>/dev/null)" ]; then \
		printf "$(GREEN)✓ Linking $(NAME)...$(RESET)\n"; \
		$(CC) $(CFLAGS) $(OBJS) $(LIBFT)/libft.a $(MLX) -o $(NAME); \
		printf "$(GREEN)$(BOLD)✓ $(NAME) compiled successfully!$(RESET)\n\n"; \
	else \
		printf "$(GREEN)✓ Nothing to be done for $(NAME)$(RESET)\n\n"; \
	fi

_compile_bonus:	libft libmlx libminiaudio $(OBJS_BONUS)
	@if [ ! -f $(NAME_BONUS) ]; then \
		printf "$(GREEN)✓ Linking $(NAME_BONUS)...$(RESET)\n"; \
		$(CC) $(CFLAGS) $(OBJS_BONUS) $(LIBFT)/libft.a $(MLX) -o $(NAME_BONUS); \
		printf "$(GREEN)$(BOLD)✓ $(NAME_BONUS) compiled successfully!$(RESET)\n\n"; \
	elif [ $(NAME_BONUS) -ot $(LIBFT)/libft.a ] || [ $(NAME_BONUS) -ot $(LIBMLX)/build/libmlx42.a ] || [ -n "$(shell find $(OBJPATH_B) -name '*.o' -newer $(NAME_BONUS) 2>/dev/null)" ]; then \
		printf "$(GREEN)✓ Linking $(NAME_BONUS)...$(RESET)\n"; \
		$(CC) $(CFLAGS) $(OBJS_BONUS) $(LIBFT)/libft.a $(MLX) -o $(NAME_BONUS); \
		printf "$(GREEN)$(BOLD)✓ $(NAME_BONUS) compiled successfully!$(RESET)\n\n"; \
	else \
		printf "$(GREEN)✓ Nothing to be done for $(NAME_BONUS)$(RESET)\n\n"; \
	fi

$(OBJPATH)/%.o:	%.c
	@mkdir -p $(@D)
	@printf "$(DARK_GRAY)\rCompiling %-50s$(RESET)" "$<"
	@$(CC) $(CFLAGS) $(BONUS_FLAG) $(INCLUDES) -o $@ -c $< && \
		printf "$(GREEN)\r✓ Compiled %-50s$(RESET)\n" "$<" || \
		(printf "$(RED)\r✗ Failed compiling %-50s$(RESET)\n" "$<" && exit 1)

$(OBJPATH_B)/%.o:	%.c
	@mkdir -p $(@D)
	@printf "$(DARK_GRAY)\rCompiling %-50s$(RESET)" "$<"
	@$(CC) $(CFLAGS) $(BONUS_FLAG) $(INCLUDES) -o $@ -c $< && \
		printf "$(GREEN)\r✓ Compiled %-50s$(RESET)\n" "$<" || \
		(printf "$(RED)\r✗ Failed compiling %-50s$(RESET)\n" "$<" && exit 1)

clean:
	@printf "$(YELLOW)Cleaning object files...$(RESET)\n"
	@$(RM) $(OBJPATH) $(OBJPATH_B)
	@if [ -f "$(LIBFT)/Makefile" ]; then \
		$(MAKE) -C $(LIBFT) clean > /dev/null 2>&1; \
	fi
	@if [ -d "$(LIBMLX)/build" ]; then \
		$(MAKE) -C $(LIBMLX)/build clean > /dev/null 2>&1; \
	fi
	@printf "$(GREEN)✓ Clean complete$(RESET)\n"

fclean:		clean
	@printf "$(YELLOW)Removing binaries...$(RESET)\n"
	@$(RM) $(NAME) $(NAME_BONUS)
	@$(RM) $(LIBPATH)
	@printf "$(GREEN)✓ Full clean complete$(RESET)\n"

libclean:
	@printf "$(YELLOW)Removing libraries...$(RESET)\n"
	@$(RM) $(LIBPATH)
	@printf "$(GREEN)✓ Libraries removed$(RESET)\n"

re:			fclean all

relib:		libclean libft libmlx miniaudio

libs/libft/Makefile:
	@mkdir -p $(LIBPATH)
	@printf "$(YELLOW)Cloning libft...$(RESET)\n"
	@git clone https://github.com/marcnava-42cursus/libft $(LIBFT) > /dev/null 2>&1
	@printf "$(GREEN)✓ libft cloned$(RESET)\n"

libs/minilibx/CMakeLists.txt:
	@mkdir -p $(LIBPATH)
	@printf "$(YELLOW)Cloning minilibx...$(RESET)\n"
	@git clone https://github.com/codam-coding-college/MLX42.git $(LIBMLX) > /dev/null 2>&1
	@printf "$(GREEN)✓ minilibx cloned$(RESET)\n"

libs/miniaudio.h:
	@printf "$(YELLOW)Downloading miniaudio.h...$(RESET)\n"
	@curl -fsSL -o libs/miniaudio.h https://raw.githubusercontent.com/mackron/miniaudio/master/miniaudio.h

libmlx:	$(LIBMLX)/CMakeLists.txt
	@current_interval="$(MLX_SWAP)"; \
	stored_interval=""; \
	if [ -f "$(MLX_SWAP_STAMP)" ]; then stored_interval=$$(cat "$(MLX_SWAP_STAMP)"); fi; \
	if [ -f "$(LIBMLX)/build/libmlx42.a" ] && \
	   [ "$(LIBMLX)/CMakeLists.txt" -ot "$(LIBMLX)/build/libmlx42.a" ] && \
	   [ "$$stored_interval" = "$$current_interval" ]; then \
		printf "$(GREEN)✓ Nothing to be done for minilibx$(RESET)\n"; \
	else \
		printf "$(YELLOW)Compiling minilibx...$(RESET)\n"; \
		if cmake $(LIBMLX) -B $(LIBMLX)/build $(MLX_CMAKE_FLAGS) > /dev/null 2>&1 && \
		   $(MAKE) -s -C $(LIBMLX)/build -j4 > /dev/null 2>&1; then \
			printf "$(GREEN)✓ minilibx compiled$(RESET)\n"; \
			printf "$$current_interval" > "$(MLX_SWAP_STAMP)"; \
		else \
			printf "$(RED)✗ Failed compiling minilibx$(RESET)\n"; \
			exit 1; \
		fi; \
	fi

libft:	$(LIBFT)/Makefile
	@if [ -f "$(LIBFT)/libft.a" ] && [ "$(LIBFT)/Makefile" -ot "$(LIBFT)/libft.a" ]; then \
		printf "$(GREEN)✓ Nothing to be done for libft$(RESET)\n"; \
	else \
		printf "$(YELLOW)Compiling libft...$(RESET)\n"; \
		if $(MAKE) -s -C $(LIBFT) > /dev/null 2>&1; then \
			printf "$(GREEN)✓ libft compiled$(RESET)\n"; \
		else \
			printf "$(RED)✗ Failed compiling libft$(RESET)\n"; \
			exit 1; \
		fi; \
	fi

libminiaudio:	libs/miniaudio.h
	@printf "$(GREEN)✓ miniaudio downloaded$(RESET)\n"

reload:	all
	./cub3D maps/example.cub

reload_bonus:	bonus
	./cub3D_bonus maps/exampleb.cub

.PHONY:		all bonus clean fclean libclean re relib libft libmlx reload reload_bonus
