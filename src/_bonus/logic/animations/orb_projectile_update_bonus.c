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
	if (row[x] != '0')
		return ;
	row[x] = game->orb.payload;
}

static void	resolve_orb_arrival(t_game *game)
{
	if (game->orb.mode == ORB_MODE_TAKE)
		store_block_in_inventory(&game->cub_data.player, game->orb.payload);
	else if (game->orb.mode == ORB_MODE_PLACE)
		place_block_on_arrival(game);
	game->orb.active = false;
	game->orb.mode = ORB_MODE_NONE;
	game->orb.payload = '\0';
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
	if (game->orb.mode == ORB_MODE_TAKE)
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
