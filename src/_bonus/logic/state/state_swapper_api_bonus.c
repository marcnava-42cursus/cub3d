/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_swapper_api_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 04:49:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 04:49:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "structs.h"
#include "logic_bonus.h"

int	get_elevator_index(char elevator_id)
{
	const char	*set;
	char		*pos;

	set = "!\"·$%&/()=?¿";
	pos = ft_strchr(set, elevator_id);
	if (!pos)
		return (-1);
	return ((int)(pos - set));
}

void	place_breakable_block(t_game *game)
{
	t_rayhit	hit;
	char		cell;
	char		block;
	int			target[2];

	if (!state_can_place_block_now_bonus(game))
		return ;
	if (!state_resolve_place_target_bonus(game, &hit, &cell, target))
		return ;
	block = consume_inventory_block(&game->cub_data.player);
	if (block == '\0')
		return ;
	if (!state_begin_block_creation_bonus(game, target[0], target[1], block))
		return ;
	if (game->menu.options.debug_mode)
		printf("Started block creation at (%d, %d)\n", target[0], target[1]);
}

void	test_break_wall_in_front(t_game *game)
{
	t_rayhit	hit;
	vertex_t	start;
	char		cell;

	if (!game)
		return ;
	if (game->menu.options.debug_mode)
		printf("Attempting to break block\n");
	start.x = game->cub_data.player.x * WORLDMAP_TILE_SIZE;
	start.y = game->cub_data.player.y * WORLDMAP_TILE_SIZE;
	hit = raycast_world(&game->cub_data.map, start, game->cub_data.player.angle,
			BREAK_BLOCK_DISTANCE * WORLDMAP_TILE_SIZE);
	if (!hit.hit)
		return ;
	cell = game->cub_data.map.grid[hit.cell[1]][hit.cell[0]];
	if (state_handle_elevator_orb_take_bonus(game, &hit, cell))
		return ;
	if (cell == 'D' || cell == 'd')
		state_modify_interactive_cell(game, hit.cell[X], hit.cell[Y]);
	else
		state_handle_breakable_cell_bonus(game, &hit, cell);
}
