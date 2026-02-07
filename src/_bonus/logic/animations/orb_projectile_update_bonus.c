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

static void	set_floor_elevator_state(t_floor *floor, int index,
			t_elevator_state state)
{
	if (!floor || index < 0 || index >= ELEVATOR_STATE_SLOTS)
		return ;
	floor->map.elevator_states[index] = state;
}

static void	place_block_on_arrival(t_game *game)
{
	int			x;
	int			y;
	char		*row;

	if (!game || !game->cub_data.map.grid)
		return ;
	x = game->orb.target_cell_x;
	y = game->orb.target_cell_y;
	if (y < 0 || y >= game->cub_data.map.height)
		return ;
	row = game->cub_data.map.grid[y];
	if (!row || x < 0 || x >= (int)ft_strlen(row))
		return ;
	if (row[x] != '0' && row[x] != ORB_GHOST_BLOCK_CELL)
		return ;
	row[x] = game->orb.payload;
}

static void	set_elevator_orb_state(t_game *game, int slot, int index,
				bool opened)
{
	if (opened)
	{
		game->cub_data.elevator_orb[slot] = true;
		game->cub_data.elevator_orb_payload[slot] = game->orb.payload;
		game->cub_data.map.elevator_states[index] = ELEVATOR_OPENED;
		set_floor_elevator_state(game->cub_data.elevator_floor_a[slot], index,
			ELEVATOR_OPENED);
		set_floor_elevator_state(game->cub_data.elevator_floor_b[slot], index,
			ELEVATOR_OPENED);
		return ;
	}
	game->cub_data.elevator_orb[slot] = false;
	game->cub_data.elevator_orb_payload[slot] = '\0';
	game->cub_data.map.elevator_states[index] = ELEVATOR_CLOSED;
	set_floor_elevator_state(game->cub_data.elevator_floor_a[slot], index,
		ELEVATOR_CLOSED);
	set_floor_elevator_state(game->cub_data.elevator_floor_b[slot], index,
		ELEVATOR_CLOSED);
}

static void	resolve_elevator_orb_arrival(t_game *game)
{
	int		slot;
	int		index;
	char	elevator_id;

	slot = game->orb.elevator_slot;
	if (slot < 0 || slot >= game->cub_data.elevator_id_count)
		return ;
	elevator_id = game->cub_data.elevator_ids[slot];
	index = get_elevator_index(elevator_id);
	if (index < 0 || index >= ELEVATOR_STATE_SLOTS)
		return ;
	set_elevator_orb_state(game, slot, index, game->orb.elevator_place);
}

static void	resolve_orb_arrival(t_game *game)
{
	if (game->orb.mode == ORB_MODE_TAKE && !game->orb.elevator_place)
		store_block_in_inventory(&game->cub_data.player, game->orb.payload);
	if (game->orb.elevator_shot)
		resolve_elevator_orb_arrival(game);
	else if (game->orb.mode == ORB_MODE_PLACE)
		place_block_on_arrival(game);
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
