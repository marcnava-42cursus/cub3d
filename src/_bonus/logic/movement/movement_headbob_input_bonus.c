/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_headbob_input_bonus.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 12:01:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 12:31:35 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logic_bonus.h"
#include "render.h"
#include "structs.h"
#include "parser.h"

void	key_hook_bonus(mlx_key_data_t keydata, void *param);
void	mouse_hook_bonus(mouse_key_t button, action_t action,
			modifier_key_t mods, void *param);
void	cursor_hook_bonus(double xpos, double ypos, void *param);
bool	process_mouse_rotation_bonus(t_game *game);

float	compute_headbob_offset(t_game *game, bool is_moving,
				bool step_started, float previous_offset)
{
	float	new_offset;

	if (is_moving)
	{
		if (step_started)
			game->headbob_phase = FT_PI_2;
		new_offset = sinf(game->headbob_phase) * HEADBOB_AMPLITUDE;
		game->headbob_phase += HEADBOB_SPEED * (float)game->mlx->delta_time;
		while (game->headbob_phase > TWO_PI)
			game->headbob_phase -= TWO_PI;
		return (new_offset);
	}
	new_offset = recover_to_zero(previous_offset,
			HEADBOB_RECOVER_SPEED * (float)game->mlx->delta_time);
	if (new_offset == 0.0f)
		game->headbob_phase = 0.0f;
	return (new_offset);
}

bool	apply_headbob_pitch(t_game *game, bool is_moving, bool step_started,
		float previous_offset)
{
	float	new_offset;
	float	max_pitch;
	float	prev_pitch;

	if (!game || !game->mlx)
		return (false);
	new_offset = compute_headbob_offset(game, is_moving, step_started,
			previous_offset);
	game->headbob_offset = new_offset;
	prev_pitch = game->cub_data.player.pitch;
	max_pitch = game->double_buffer[NEXT]->height * 0.35f;
	game->cub_data.player.pitch = clamp(prev_pitch + new_offset,
			-max_pitch, max_pitch);
	game->headbob_applied = game->cub_data.player.pitch - prev_pitch;
	return (fabsf(new_offset - previous_offset) > 0.0001f);
}

/**
 * @brief Processes movement input and updates game state
 *
 * This function checks all movement-related key states and calls the
 * appropriate movement/rotation functions from move.c and rotation.c.
 *
 * @param game Game state structure
 * @return bool True if any movement occurred
*/
void	read_move_axes(t_game *game, float *forward, float *strafe)
{
	*forward = 0.0f;
	*strafe = 0.0f;
	if (game->key_w_pressed)
		*forward += 1.0f;
	if (game->key_s_pressed)
		*forward -= 1.0f;
	if (game->key_d_pressed)
		*strafe += 1.0f;
	if (game->key_a_pressed)
		*strafe -= 1.0f;
	*forward += game->controller.move_forward;
	*strafe += game->controller.move_strafe;
	*forward = clamp(*forward, -1.0f, 1.0f);
	*strafe = clamp(*strafe, -1.0f, 1.0f);
}

bool	apply_translation_input(t_game *game, float forward, float strafe)
{
	float	move_x;
	float	move_y;
	float	len;
	float	speed;
	float	angle;

	if (forward == 0.0f && strafe == 0.0f)
		return (false);
	angle = game->cub_data.player.angle;
	move_x = cosf(angle) * forward + cosf(angle + FT_PI_2) * strafe;
	move_y = sinf(angle) * forward + sinf(angle + FT_PI_2) * strafe;
	len = sqrtf(move_x * move_x + move_y * move_y);
	if (len > 1.0f)
	{
		move_x /= len;
		move_y /= len;
	}
	speed = game->move_speed * (float)game->mlx->delta_time;
	ATTEMPT_MOVE(game, move_x * speed, move_y * speed);
	return (true);
}

bool	apply_rotation_input(t_game *game)
{
	bool	moved;
	float	turn;

	moved = false;
	if (game->key_left_pressed)
		moved = (rotate_player(game, false), true);
	if (game->key_right_pressed)
		moved = (rotate_player(game, true), true);
	turn = game->controller.turn;
	if (turn > EPSILON || turn < -EPSILON)
	{
		game->cub_data.player.angle += game->rot_speed
			* (float)game->mlx->delta_time * turn;
		game->cub_data.player.angle = normalize_angle(
				game->cub_data.player.angle);
		moved = true;
	}
	return (moved);
}
