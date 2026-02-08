/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 12:01:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 02:47:18 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logic_bonus.h"
#include "render.h"

#include "structs.h"

#include "parser.h"

void	refresh_key_states_bonus(t_game *game);
void	key_hook_bonus(mlx_key_data_t keydata, void *param);
void	mouse_hook_bonus(mouse_key_t button, action_t action,
			modifier_key_t mods, void *param);
void	cursor_hook_bonus(double xpos, double ypos, void *param);
bool	process_mouse_rotation_bonus(t_game *game);

#define HEADBOB_AMPLITUDE 5.0f
#define HEADBOB_SPEED 14.0f
#define HEADBOB_RECOVER_SPEED 30.0f
#define JUMP_PITCH_IMPULSE 700.0f
#define GRAVITY 2200.0f

bool	update_motion_frame(t_game *game, float prev_headbob,
				float prev_jump)
{
	bool		moved;
	bool		mouse_rotated;
	bool		headbob_changed;
	bool		jump_changed;
	vertex_t	prev;

	if (game->mlx->delta_time <= 0.0)
	{
		clear_pitch_offsets(game);
		reset_headbob_state(game);
		reset_jump_state(game);
		return (false);
	}
	clear_pitch_offsets(game);
	prev = capture_player_position(game);
	moved = process_movement_input(game);
	mouse_rotated = process_mouse_rotation_bonus(game);
	headbob_changed = update_step_audio_and_headbob(game,
			player_translated_since(game, prev), prev_headbob);
	jump_changed = update_jump_pitch(game, prev_jump);
	if (moved || mouse_rotated || headbob_changed || jump_changed)
		handle_movement_rendering(game);
	return (true);
}

bool	handle_modal_overlay_loop(t_game *game)
{
	if (!is_config_modal_open(game))
		return (false);
	clear_pitch_offsets(game);
	audio_step_update_loop(false);
	reset_headbob_state(game);
	reset_jump_state(game);
	update_config_modal(game);
	return (true);
}

/**
 * @brief Main game loop function for continuous movement processing
 *
 * This function is called every frame by MLX and serves as the central
 * coordinator for the game loop. It:
 * 1. Uses MLX delta time for frame-independent movement
 * 2. Refreshes key states (from input.c)
 * 3. Processes movement input (delegates to move.c and rotation.c)
 * 4. Processes mouse rotation
 * 5. Updates rendering if movement occurred
 *
 * @param param Void pointer to game structure (casted internally)
 */
void	update_game_loop_bonus(void *param)
{
	t_game			*game;
	float			prev_headbob;
	float			prev_jump;
	double			next_tick;
	int				last_limit;

	next_tick = 0.0;
	last_limit = -2;
	game = (t_game *)param;
	if (!game || !game->mlx)
		return ;
	prev_headbob = game->headbob_offset;
	prev_jump = game->jump_offset;
	if (handle_modal_loop(game)
		|| throttle_fps_loop(game, &next_tick, &last_limit))
		return ;
	refresh_key_states_bonus(game);
	if (handle_modal_overlay_loop(game))
		return ;
	fps_overlay_update(game);
	update_motion_frame(game, prev_headbob, prev_jump);
}

/**
 * @brief Initializes the player's starting angle based on orientation
 *
 * Converts the cardinal direction (N, S, E, W) from the map file
 * into a radian angle for the 3D engine.
 *
 * @param game Game state structure
 */
void	init_player_angle(t_game *game)
{
	if (!game)
		return ;
	if (game->cub_data.player.orientation == NORTH)
		game->cub_data.player.angle = -FT_PI / 2.0f;
	else if (game->cub_data.player.orientation == SOUTH)
		game->cub_data.player.angle = FT_PI / 2.0f;
	else if (game->cub_data.player.orientation == EAST)
		game->cub_data.player.angle = 0.0f;
	else if (game->cub_data.player.orientation == WEST)
		game->cub_data.player.angle = FT_PI;
}

void	init_movement_runtime(t_game *game)
{
	game->movement_lock_until = 0.0;
	game->last_teleport_time = -10.0;
	game->last_teleport_id = '\0';
	game->mouse_initialized = false;
	game->mouse_delta_accumulated = 0.0f;
	game->mouse_delta_accumulated_y = 0.0f;
	game->mouse_sensitivity = config_mouse_sens_value(
			game->menu.options.mouse_sens);
	game->last_mouse_x = 0.0;
	game->last_mouse_y = 0.0;
	game->headbob_phase = 0.0f;
	game->headbob_offset = 0.0f;
	game->headbob_applied = 0.0f;
	game->jump_active = false;
	game->jump_offset = 0.0f;
	game->jump_velocity = 0.0f;
	game->jump_applied = 0.0f;
	game->last_player_angle = game->cub_data.player.angle;
	game->last_grid_x = -1;
	game->last_grid_y = -1;
}
