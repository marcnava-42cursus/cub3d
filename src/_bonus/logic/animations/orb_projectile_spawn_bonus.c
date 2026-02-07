/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orb_projectile_spawn_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/07 02:10:45 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "logic_bonus.h"

static void	orb_projectile_activate(t_game *game, t_orb_mode mode,
		float start_x, float start_y)
{
	if (!game)
		return ;
	game->orb.active = true;
	game->orb.mode = mode;
	game->orb.x = start_x;
	game->orb.y = start_y;
	game->orb.needs_redraw = true;
}

bool	orb_projectile_start_take(t_game *game, int cell_x, int cell_y,
	char block)
{
	if (!game || game->orb.active)
		return (false);
	orb_projectile_activate(game, ORB_MODE_TAKE,
		(float)cell_x + 0.5f, (float)cell_y + 0.5f);
	bonus_audio_update_orb_volume(game);
	bonus_audio_play_orb_launch();
	game->orb.payload = block;
	game->orb.target_cell_x = -1;
	game->orb.target_cell_y = -1;
	game->orb.target_x = game->cub_data.player.x;
	game->orb.target_y = game->cub_data.player.y;
	game->orb.elevator_shot = false;
	game->orb.elevator_place = false;
	game->orb.elevator_slot = -1;
	return (true);
}

bool	orb_projectile_start_place(t_game *game, int target_x, int target_y,
		char block)
{
	if (!game || game->orb.active)
		return (false);
	if (!orb_projectile_spawn_ghost(game, target_x, target_y))
		return (false);
	orb_projectile_activate(game, ORB_MODE_PLACE,
		game->cub_data.player.x, game->cub_data.player.y);
	bonus_audio_update_orb_volume(game);
	bonus_audio_play_orb_launch();
	game->orb.payload = block;
	game->orb.target_cell_x = target_x;
	game->orb.target_cell_y = target_y;
	game->orb.target_x = (float)target_x + 0.5f;
	game->orb.target_y = (float)target_y + 0.5f;
	game->orb.elevator_shot = false;
	game->orb.elevator_place = false;
	game->orb.elevator_slot = -1;
	return (true);
}

bool	orb_projectile_start_elevator_place(t_game *game, int target_x,
		int target_y, char block, int elevator_slot)
{
	if (!game || game->orb.active)
		return (false);
	orb_projectile_activate(game, ORB_MODE_TAKE,
		game->cub_data.player.x, game->cub_data.player.y);
	bonus_audio_update_orb_volume(game);
	bonus_audio_play_orb_launch();
	game->orb.payload = block;
	game->orb.target_cell_x = target_x;
	game->orb.target_cell_y = target_y;
	game->orb.target_x = (float)target_x + 0.5f;
	game->orb.target_y = (float)target_y + 0.5f;
	game->orb.elevator_shot = true;
	game->orb.elevator_place = true;
	game->orb.elevator_slot = elevator_slot;
	return (true);
}
