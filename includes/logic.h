/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 18:07:12 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/06 13:03:50 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGIC_H
# define LOGIC_H

# include "structs.h"
# include "libft.h"
# include "render.h"

# define TWO_PI (2.0f * FT_PI)
# define MAX_MOUSE_DELTA 100.0f

/* Terminal control codes */
# define ANSI_CLEAR_SCREEN "\033[2J\033[H"

/* Angle thresholds for player direction display (in radians) */
# define ANGLE_RIGHT_MIN (-FT_PI / 4.0f)
# define ANGLE_RIGHT_MAX (FT_PI / 4.0f)
# define ANGLE_DOWN_MIN (FT_PI / 4.0f)
# define ANGLE_DOWN_MAX (3.0f * FT_PI / 4.0f)
# define ANGLE_LEFT_MIN (3.0f * FT_PI / 4.0f)
# define ANGLE_LEFT_MAX (-3.0f * FT_PI / 4.0f)

/* Player direction symbols */
# define DIR_RIGHT '>'
# define DIR_DOWN 'v'
# define DIR_LEFT '<'
# define DIR_UP '^'

# ifdef BONUS
// Modal defines
#  define CONFIG_MODAL_BG_COLOR 0x000000C0
#  define CONFIG_MODAL_PANEL_COLOR 0x1E1E1ED0
#  define CONFIG_MODAL_BORDER_COLOR 0xCCCCCCFF
#  define CONFIG_MODAL_BORDER_THICKNESS 2

#  define TAB_HEIGHT 40
#  define TAB_INACTIVE_COLOR 0x333333FF
#  define TAB_ACTIVE_COLOR 0x1E1E1ED0
#  define TAB_BORDER_COLOR 0x999999FF
#  define TAB_SELECTED_BORDER_COLOR 0xFFFFFFFF
#  define TAB_SELECTED_BORDER_THICKNESS 3

#  define CONFIG_MODAL_QUIT_TEXT "QUIT (Q)"
#  define CONFIG_MODAL_QUIT_BTN_COLOR 0x555555FF
#  define CONFIG_MODAL_QUIT_BTN_HOVER_COLOR 0x444444FF
#  define CONFIG_MODAL_QUIT_BTN_MARGIN 24
#  define CONFIG_MODAL_QUIT_BTN_W 160
#  define CONFIG_MODAL_QUIT_BTN_H 52
#  define CONFIG_MODAL_QUIT_HOLD_SECONDS 1.2
#  define CONFIG_MODAL_QUIT_PROGRESS_COLOR 0xB33000FF
#  define CONFIG_MODAL_QUIT_PROGRESS_THICKNESS 4

#  define ICON_COLOR 0xFFFFFFFF
#  define CONFIG_MODAL_OPTION_TOP 80
#  define CONFIG_MODAL_OPTION_LEFT 32
#  define CONFIG_MODAL_OPTION_RIGHT 32
#  define CONFIG_MODAL_OPTION_LINE 40
#  define CONFIG_MODAL_OPTION_BOX 22
# endif

typedef struct s_game t_game;

// Input
void	refresh_key_states(t_game *game);
void	key_hook(mlx_key_data_t keydata, void *param);
void	mouse_hook(mouse_key_t button, action_t action,
			modifier_key_t mods, void *param);
void	cursor_hook(double xpos, double ypos, void *param);
bool	process_mouse_rotation(t_game *game);

// Timing
void	init_player_parameters(t_game *game);

// Movement
void	move_forward(t_game *game, bool forward);
void	move_strafe(t_game *game, bool right);
void	rotate_player(t_game *game, bool right);

// Collision
bool	is_cell_blocking(t_game *game, int cell_x, int cell_y);
bool	collides_with_wall(t_game *game, float x, float y);
void	attempt_move(t_game *game, float step_x, float step_y);

# ifdef BONUS

bool	is_cell_blocking_bonus(t_game *game, int cell_x, int cell_y);
bool	collides_with_wall_bonus(t_game *game, float x, float y);
void	attempt_move_bonus(t_game *game, float step_x, float step_y);
#  define is_cell_blocking is_cell_blocking_bonus
#  define collides_with_wall collides_with_wall_bonus
#  define attempt_move attempt_move_bonus

// Input bonus
void	refresh_key_states_bonus(t_game *game);
void	key_hook_bonus(mlx_key_data_t keydata, void *param);
void	mouse_hook_bonus(mouse_key_t button, action_t action,
			modifier_key_t mods, void *param);
void	cursor_hook_bonus(double xpos, double ypos, void *param);
bool	process_mouse_rotation_bonus(t_game *game);

// Movement bonus
void	update_game_loop_bonus(void *param);
void	init_movement_system_bonus(t_game *game);

#  define refresh_key_states refresh_key_states_bonus
#  define key_hook key_hook_bonus
#  define mouse_hook mouse_hook_bonus
#  define cursor_hook cursor_hook_bonus
#  define process_mouse_rotation process_mouse_rotation_bonus
#  define update_game_loop update_game_loop_bonus
#  define init_movement_system init_movement_system_bonus

// Inventory
bool	player_has_block(const t_player *player);
bool	store_block_in_inventory(t_player *player, char block);
char	consume_inventory_block(t_player *player);

// Config modal UI (prototype)
void	set_config_modal_visible(t_game *game, bool visible);
void	toggle_config_modal(t_game *game);
bool	is_config_modal_open(const t_game *game);
void	update_config_modal(t_game *game);
void	config_modal_handle_key(t_game *game, mlx_key_data_t keydata);

# endif

// Debug / Terminal Output
void	print_controls(void);
void	print_map_2d(t_game *game);
void	print_player_position_inline(t_game *game, int grid_x, int grid_y);
void	handle_debug_map_update(t_game *game, int grid_x, int grid_y);

// Debug / On-Screen Display
void	init_crosshair(t_game *game);

# ifdef BONUS

// Test / Prototype
void	test_break_wall_in_front(t_game *game);
void	place_breakable_block(t_game *game);

# endif

// Loop / Update
void	update_player_position(t_game *game);
void	update_game_loop(void *param);
void	init_movement_system(t_game *game);

#endif
