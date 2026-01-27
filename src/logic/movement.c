/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 12:01:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/27 19:25:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logic.h"
#include "render.h"
#include "structs.h"

static bool	handle_translation(t_game *game)
{
	float	forward;
	float	strafe;
	float	move_x;
	float	move_y;
	float	len;
	float	speed;
	float	angle;

	if (!game || !game->mlx)
		return (false);
	forward = 0.0f;
	strafe = 0.0f;
	if (game->key_w_pressed)
		forward += 1.0f;
	if (game->key_s_pressed)
		forward -= 1.0f;
	if (game->key_d_pressed)
		strafe += 1.0f;
	if (game->key_a_pressed)
		strafe -= 1.0f;
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
	attempt_move(game, move_x * speed, move_y * speed);
	return (true);
}

static bool	handle_rotation(t_game *game)
{
	bool	moved;

	moved = false;
	if (game->key_left_pressed)
	{
		rotate_player(game, false);
		moved = true;
	}
	if (game->key_right_pressed)
	{
		rotate_player(game, true);
		moved = true;
	}
	return (moved);
}

static bool	process_movement_input(t_game *game)
{
	bool	moved;

	if (!game)
		return (false);
	moved = handle_translation(game);
	if (handle_rotation(game))
		moved = true;
	return (moved);
}

void	update_game_loop(void *param)
{
	t_game	*game;
	bool	moved;
	bool	mouse_rotated;

	game = (t_game *)param;
	if (!game || !game->mlx)
		return ;
	refresh_key_states(game);
	fps_overlay_update(game);
	if (game->mlx->delta_time <= 0.0)
		return ;
	moved = process_movement_input(game);
	mouse_rotated = process_mouse_rotation(game);
	if (moved || mouse_rotated)
		movement_handle_elevator(game);
}
