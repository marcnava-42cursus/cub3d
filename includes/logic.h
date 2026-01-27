/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 18:07:12 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/27 02:05:23 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGIC_H
# define LOGIC_H

# include <stdbool.h>
# include "MLX42/MLX42.h"
# include "libft.h"

# define BREAK_BLOCK_DISTANCE 4
# define PLACE_BLOCK_DISTANCE 20

// Orientaciones del jugador
typedef enum e_orientation
{
	NORTH = 'N',
	SOUTH = 'S',
	EAST = 'E',
	WEST = 'W'
}	t_orientation;

typedef enum e_player_state
{
	STATE_EMPTY = 0,
	STATE_THROW = 1,
	STATE_TAKE = 2,
	STATE_HOLD = 3
}	t_player_state;

# ifdef BONUS
typedef enum e_orb_mode
{
	ORB_MODE_NONE = 0,
	ORB_MODE_TAKE = 1,
	ORB_MODE_PLACE = 2
}	t_orb_mode;

typedef struct s_orb_projectile
{
	bool		active;
	t_orb_mode	mode;
	float		x;
	float		y;
	float		target_x;
	float		target_y;
	float		speed;
	char		payload;
	int			target_cell_x;
	int			target_cell_y;
	bool		needs_redraw;
	float		last_draw_x;
	float		last_draw_y;
	bool		last_draw_active;
}	t_orb_projectile;

#  define ORB_PROJECTILE_DEFAULT_SPEED 6.0f
# endif

# define TWO_PI (2.0f * FT_PI)
# define MOUSE_SMOOTHING 10.0f
# define EPSILON 0.0f

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

typedef struct s_game t_game;
typedef struct s_player t_player;

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
void	pitch_player(t_game *game, bool up);

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

// Orb projectile
void	init_orb_projectile_bonus(t_game *game);
bool	orb_projectile_is_active(const t_game *game);
bool	orb_projectile_start_take(t_game *game, int cell_x, int cell_y,
			char block);
bool	orb_projectile_start_place(t_game *game, int target_x, int target_y,
			char block);
bool	orb_projectile_update(t_game *game, float delta_time);
bool	orb_projectile_needs_redraw(const t_game *game);
void	orb_projectile_mark_drawn(t_game *game);

// Config modal UI (prototype)
void	set_config_modal_visible(t_game *game, bool visible);
void	toggle_config_modal(t_game *game);
bool	is_config_modal_open(const t_game *game);
void	update_config_modal(t_game *game);
void	config_modal_handle_key(t_game *game, mlx_key_data_t keydata);
void	fps_overlay_update(t_game *game);

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
