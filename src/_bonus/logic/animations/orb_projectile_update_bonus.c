/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orb_projectile_update_bonus.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/07 23:38:20 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#include "logic_bonus.h"

static void	resolve_orb_arrival(t_game *game)
{
	if (game->orb.mode == ORB_MODE_TAKE && !game->orb.elevator_place)
		store_block_in_inventory(&game->cub_data.player, game->orb.payload);
	if (game->orb.elevator_shot)
		orb_projectile_apply_elevator_arrival_bonus(game);
	else if (game->orb.mode == ORB_MODE_PLACE)
		orb_projectile_place_block_on_arrival_bonus(game);
	audio_orb_stop();
	game->orb.active = false;
	orb_projectile_clear_ghost(game);
	game->orb.mode = ORB_MODE_NONE;
	game->orb.payload = '\0';
	game->orb.elevator_shot = false;
	game->orb.elevator_place = false;
	game->orb.elevator_slot = -1;
	game->orb.needs_redraw = true;
}

static void	update_orb_target_for_take(t_game *game)
{
	if (game->orb.mode == ORB_MODE_TAKE && !game->orb.elevator_place)
	{
		game->orb.target_x = game->cub_data.player.x;
		game->orb.target_y = game->cub_data.player.y;
	}
}

static bool	finish_orb_if_reached(t_game *game, float distance, float step)
{
	if (distance <= 0.0001f)
	{
		resolve_orb_arrival(game);
		return (true);
	}
	if (distance > step)
		return (false);
	game->orb.x = game->orb.target_x;
	game->orb.y = game->orb.target_y;
	audio_orb_update_volume(game);
	resolve_orb_arrival(game);
	return (true);
}

bool	orb_projectile_update(t_game *game, float delta_time)
{
	float	dx;
	float	dy;
	float	distance;
	float	step;

	if (!game || !game->orb.active || delta_time <= 0.0f)
		return (false);
	update_orb_target_for_take(game);
	dx = game->orb.target_x - game->orb.x;
	dy = game->orb.target_y - game->orb.y;
	distance = sqrtf(dx * dx + dy * dy);
	step = game->orb.speed * delta_time;
	if (finish_orb_if_reached(game, distance, step))
		return (true);
	game->orb.x += (dx / distance) * step;
	game->orb.y += (dy / distance) * step;
	audio_orb_update_volume(game);
	game->orb.needs_redraw = true;
	return (true);
}
