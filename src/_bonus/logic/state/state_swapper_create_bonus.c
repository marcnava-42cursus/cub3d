/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_swapper_create_bonus.c                       :+:      :+:    :+:   */
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

static bool	get_pending_block_row_bonus(t_game *game,
			t_living_block *block, char **row)
{
	if (!game || !block || !row || !game->cub_data.map.grid)
		return (false);
	if (!block->is_creating || !block->anims
		|| !block->anims[ANIM_CREATE].finished)
		return (false);
	if (block->pending_y < 0 || block->pending_y >= game->cub_data.map.height)
		return (false);
	*row = game->cub_data.map.grid[block->pending_y];
	if (!*row || block->pending_x < 0
		|| block->pending_x >= (int)ft_strlen(*row))
		return (false);
	return (true);
}

static void	reset_pending_block_bonus(t_living_block *block)
{
	block->is_creating = false;
	block->pending_payload = '\0';
	block->pending_x = -1;
	block->pending_y = -1;
	anim_start(&block->anims[ANIM_CREATE]);
}

void	update_creating_block_state(t_game *game)
{
	t_living_block	*block;
	char			*row;
	char			block_cell;

	block = &game->cub_data.block;
	if (!get_pending_block_row_bonus(game, block, &row))
		return ;
	if (block->pending_payload == '\0')
		block_cell = '2';
	else
		block_cell = block->pending_payload;
	if (row[block->pending_x] == ORB_GHOST_BLOCK_CELL)
		row[block->pending_x] = block_cell;
	reset_pending_block_bonus(block);
}
