/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_swapper_place_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 05:30:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 05:30:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "structs.h"
#include "logic_bonus.h"

bool	state_handle_elevator_orb_place_bonus(t_game *game,
			t_rayhit *hit, char cell)
{
	int		slot;
	char	payload;

	if (!state_is_elevator_bonus(cell))
		return (false);
	slot = get_elevator_slot_bonus(&game->cub_data, cell);
	if (slot < 0 || game->cub_data.elevator_orb[slot])
		return (true);
	payload = consume_inventory_block(&game->cub_data.player);
	if (payload == '\0')
		return (true);
	if (!orb_projectile_start_elevator_place(game, hit->cell, payload, slot))
	{
		store_block_in_inventory(&game->cub_data.player, payload);
		return (true);
	}
	if (game->menu.options.debug_mode)
		printf("Injected orb into elevator '%c'\n", cell);
	return (true);
}

bool	state_get_place_hit_bonus(t_game *game, t_rayhit *hit, char *cell)
{
	vertex_t	start;

	start.x = game->cub_data.player.x * WORLDMAP_TILE_SIZE;
	start.y = game->cub_data.player.y * WORLDMAP_TILE_SIZE;
	*hit = raycast_world(&game->cub_data.map, start,
			game->cub_data.player.angle,
			PLACE_BLOCK_DISTANCE * WORLDMAP_TILE_SIZE);
	if (!hit->hit)
		return (false);
	if (!state_target_inside_map_bonus(game, hit->cell[0], hit->cell[1]))
		return (false);
	*cell = game->cub_data.map.grid[hit->cell[1]][hit->cell[0]];
	return (true);
}

bool	state_begin_block_creation_bonus(t_game *game,
			int target_x, int target_y, char cell)
{
	if (!orb_projectile_spawn_ghost(game, target_x, target_y))
	{
		store_block_in_inventory(&game->cub_data.player, cell);
		return (false);
	}
	game->cub_data.block.is_creating = true;
	game->cub_data.block.pending_payload = cell;
	game->cub_data.block.pending_x = target_x;
	game->cub_data.block.pending_y = target_y;
	anim_start(&game->cub_data.block.anims[ANIM_CREATE]);
	game->cub_data.player.state = STATE_THROW;
	return (true);
}

bool	state_can_place_block_now_bonus(t_game *game)
{
	if (!game)
		return (false);
	if (game->menu.options.debug_mode)
		printf("Attempting to place block\n");
	if (orb_projectile_is_active(game))
		return (false);
	if (game->cub_data.block.is_creating)
		return (false);
	return (player_has_block(&game->cub_data.player));
}

bool	state_resolve_place_target_bonus(t_game *game,
			t_rayhit *hit, char *cell, int target[2])
{
	if (!state_get_place_hit_bonus(game, hit, cell))
		return (false);
	if (state_handle_elevator_orb_place_bonus(game, hit, *cell))
		return (false);
	if (*cell != '1' && *cell != '2')
		return (false);
	if (!state_get_attachment_coords_bonus(game, hit, &target[0], &target[1]))
		return (false);
	return (state_is_valid_target_cell_bonus(game, target[0], target[1]));
}
