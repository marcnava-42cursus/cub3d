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
	bool	moved;

	moved = false;
	if (game->key_w_pressed)
	{
		move_forward(game, true);
		moved = true;
	}
	if (game->key_s_pressed)
	{
		move_forward(game, false);
		moved = true;
	}
	if (game->key_a_pressed)
	{
		move_strafe(game, false);
		moved = true;
	}
	if (game->key_d_pressed)
	{
		move_strafe(game, true);
		moved = true;
	}
	return (moved);
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
