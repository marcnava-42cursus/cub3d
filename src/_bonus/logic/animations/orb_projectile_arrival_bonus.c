/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orb_projectile_arrival_bonus.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 07:05:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 07:05:00 by marcnava         ###   ########.fr       */
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

void	orb_projectile_apply_elevator_arrival_bonus(t_game *game)
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

void	orb_projectile_place_block_on_arrival_bonus(t_game *game)
{
	int		x;
	int		y;
	char	*row;

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
