/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 18:07:12 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/02 16:55:02 by ivmirand         ###   ########.fr       */
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

typedef struct s_fps_overlay
{
	mlx_image_t	*label;
	double		accum;
	int			frames;
	double		last_time;
} 	t_fps_overlay;

# define TWO_PI (2.0f * FT_PI)
# define MOUSE_SMOOTHING 10.0f
# define EPSILON 0.0f

# ifdef DEBUG_FPS

#  define FPS_OVERLAY_ENABLED 1

# else

#  define FPS_OVERLAY_ENABLED 0

# endif

typedef struct s_game	t_game;
typedef struct s_player	t_player;

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

// Debug / On-Screen Display
void	init_crosshair(t_game *game);
void	fps_overlay_update(t_game *game);

// Loop / Update
void	update_player_position(t_game *game);
void	update_game_loop(void *param);
void	init_movement_system(t_game *game);

# ifdef BONUS

bool	is_cell_blocking_bonus(t_game *game, int cell_x, int cell_y);
bool	collides_with_wall_bonus(t_game *game, float x, float y);
void	attempt_move_bonus(t_game *game, float step_x, float step_y);
void	refresh_key_states_bonus(t_game *game);
void	key_hook_bonus(mlx_key_data_t keydata, void *param);
void	mouse_hook_bonus(mouse_key_t button, action_t action,
			modifier_key_t mods, void *param);
void	cursor_hook_bonus(double xpos, double ypos, void *param);
bool	process_mouse_rotation_bonus(t_game *game);
void	update_game_loop_bonus(void *param);
void	init_movement_system_bonus(t_game *game);

#  define IS_CELL_BLOCKING is_cell_blocking_bonus
#  define COLLIDES_WITH_WALL collides_with_wall_bonus
#  define ATTEMPT_MOVE attempt_move_bonus
#  define REFRESH_KEY_STATES refresh_key_states_bonus
#  define KEY_HOOK key_hook_bonus
#  define MOUSE_HOOK mouse_hook_bonus
#  define CURSOR_HOOK cursor_hook_bonus
#  define PROCESS_MOUSE_ROTATION process_mouse_rotation_bonus
#  define UPDATE_GAME_LOOP update_game_loop_bonus
#  define INIT_MOVEMENT_SYSTEM init_movement_system_bonus

# else

#  define IS_CELL_BLOCKING is_cell_blocking
#  define COLLIDES_WITH_WALL collides_with_wall
#  define ATTEMPT_MOVE attempt_move
#  define REFRESH_KEY_STATES refresh_key_states
#  define KEY_HOOK key_hook
#  define MOUSE_HOOK mouse_hook
#  define CURSOR_HOOK cursor_hook
#  define PROCESS_MOUSE_ROTATION process_mouse_rotation
#  define UPDATE_GAME_LOOP update_game_loop
#  define INIT_MOVEMENT_SYSTEM init_movement_system

# endif

#endif
