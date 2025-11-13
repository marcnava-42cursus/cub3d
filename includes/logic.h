/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 18:07:12 by marcnava          #+#    #+#             */
/*   Updated: 2025/11/12 19:01:20 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGIC_H
# define LOGIC_H

# define TWO_PI (2.0f * FT_PI)
# define MAX_DELTA_TIME 0.05
# define MAX_MOUSE_DELTA 100.0f

/* Terminal control codes */
#define ANSI_CLEAR_SCREEN "\033[2J\033[H"

/* Angle thresholds for player direction display (in radians) */
#define ANGLE_RIGHT_MIN (-FT_PI / 4.0f)
#define ANGLE_RIGHT_MAX (FT_PI / 4.0f)
#define ANGLE_DOWN_MIN (FT_PI / 4.0f)
#define ANGLE_DOWN_MAX (3.0f * FT_PI / 4.0f)
#define ANGLE_LEFT_MIN (3.0f * FT_PI / 4.0f)
#define ANGLE_LEFT_MAX (-3.0f * FT_PI / 4.0f)

/* Player direction symbols */
#define DIR_RIGHT '>'
#define DIR_DOWN 'v'
#define DIR_LEFT '<'
#define DIR_UP '^'

typedef struct s_game t_game;

// Input
void	refresh_key_states(t_game *game);
void	key_hook(mlx_key_data_t keydata, void *param);
void	cursor_hook(double xpos, double ypos, void *param);
bool	process_mouse_rotation(t_game *game);

// Timing
void	update_delta_time(t_game *game);
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
# endif

// Debug / Terminal Output
void	print_controls(void);
void	print_map_2d(t_game *game);
void	print_player_position_inline(t_game *game, int grid_x, int grid_y);
void	handle_debug_map_update(t_game *game, int grid_x, int grid_y);

// Debug / On-Screen Display
void	init_crosshair(t_game *game);

// Test / Prototype
void	test_break_wall_in_front(t_game *game);

// Loop / Update
void	update_player_position(t_game *game);
void	update_game_loop(void *param);
void	init_movement_system(t_game *game);

#endif
