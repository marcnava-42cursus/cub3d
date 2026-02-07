/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orb_projectile_update_bonus.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/22 00:00:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "logic_bonus.h"

static void	set_door_texture_state(t_game *game, bool open)
{
	t_anim			*door;
	unsigned int	frame_id;
	unsigned int	width;

	if (!game || !game->cub_data.effects.door_anims)
		return ;
	door = &game->cub_data.effects.door_anims[DOOR_OPEN];
	if (!door->atlas)
		return ;
	frame_id = 0;
	if (open)
		frame_id = 8;
	width = door->atlas->max_frame[X];
	if (width == 0)
		return ;
	door->current_frame[X] = frame_id % width;
	door->current_frame[Y] = frame_id / width;
}

static bool	has_open_elevator(t_game *game)
{
	int	i;

	if (!game)
		return (false);
	i = 0;
	while (i < game->cub_data.elevator_id_count)
	{
		if (game->cub_data.elevator_orb[i])
			return (true);
		i++;
	}
	return (false);
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

static void	resolve_elevator_orb_arrival(t_game *game)
{
	int	slot;

	slot = game->orb.elevator_slot;
	if (slot < 0 || slot >= game->cub_data.elevator_id_count)
		return ;
	if (game->orb.elevator_place)
	{
		game->cub_data.elevator_orb[slot] = true;
		game->cub_data.elevator_orb_payload[slot] = game->orb.payload;
	}
	else
	{
		game->cub_data.elevator_orb[slot] = false;
		game->cub_data.elevator_orb_payload[slot] = '\0';
	}
	set_door_texture_state(game, has_open_elevator(game));
}

static void	resolve_orb_arrival(t_game *game)
{
	if (game->orb.mode == ORB_MODE_TAKE && !game->orb.elevator_place)
		store_block_in_inventory(&game->cub_data.player, game->orb.payload);
	if (game->orb.elevator_shot)
		resolve_elevator_orb_arrival(game);
	else if (game->orb.mode == ORB_MODE_PLACE)
		place_block_on_arrival(game);
	game->orb.active = false;
	orb_projectile_clear_ghost(game);
	game->orb.mode = ORB_MODE_NONE;
	game->orb.payload = '\0';
	game->orb.elevator_shot = false;
	game->orb.elevator_place = false;
	game->orb.elevator_slot = -1;
	game->orb.needs_redraw = true;
}

bool	orb_projectile_update(t_game *game, float delta_time)
{
	float	dx;
	float	dy;
	float	distance;
	float	step;

	if (!game || !game->orb.active || delta_time <= 0.0f)
		return (false);
	if (game->orb.mode == ORB_MODE_TAKE && !game->orb.elevator_place)
	{
		game->orb.target_x = game->cub_data.player.x;
		game->orb.target_y = game->cub_data.player.y;
	}
	dx = game->orb.target_x - game->orb.x;
	dy = game->orb.target_y - game->orb.y;
	distance = sqrtf(dx * dx + dy * dy);
	if (distance <= 0.0001f)
	{
		resolve_orb_arrival(game);
		return (true);
	}
	step = game->orb.speed * delta_time;
	if (distance <= step)
	{
		game->orb.x = game->orb.target_x;
		game->orb.y = game->orb.target_y;
		resolve_orb_arrival(game);
		return (true);
	}
	game->orb.x += (dx / distance) * step;
	game->orb.y += (dy / distance) * step;
	game->orb.needs_redraw = true;
	return (true);
}
