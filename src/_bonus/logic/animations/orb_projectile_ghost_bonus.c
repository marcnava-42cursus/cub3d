/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orb_projectile_ghost_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/02 00:00:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static bool	get_ghost_cell(t_game *game, int cell_x, int cell_y, char **cell)
{
	char	*row;

	if (!game || !game->cub_data.map.grid || !cell)
		return (false);
	if (cell_y < 0 || cell_y >= game->cub_data.map.height)
		return (false);
	row = game->cub_data.map.grid[cell_y];
	if (!row || cell_x < 0 || cell_x >= (int)ft_strlen(row))
		return (false);
	*cell = &row[cell_x];
	return (true);
}

bool	orb_projectile_spawn_ghost(t_game *game, int target_x, int target_y)
{
	char	*cell;

	if (!get_ghost_cell(game, target_x, target_y, &cell))
		return (false);
	if (*cell != '0')
		return (false);
	*cell = ORB_GHOST_BLOCK_CELL;
	return (true);
}

void	orb_projectile_clear_ghost(t_game *game)
{
	char	*cell;

	if (!game || game->orb.mode != ORB_MODE_PLACE)
		return ;
	if (!get_ghost_cell(game, game->orb.target_cell_x,
			game->orb.target_cell_y, &cell))
		return ;
	if (*cell == ORB_GHOST_BLOCK_CELL)
		*cell = '0';
}
