/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/06 14:12:51 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <math.h>
#include <string.h>

bool	is_cell_blocking_bonus(t_game *game, int cell_x, int cell_y)
{
	char	*row;
	int		row_len;
	char	cell;

	if (!game || !game->cub_data.map.grid)
		return (true);
	if (cell_x < 0 || cell_y < 0 || cell_y >= game->cub_data.map.height)
		return (true);
	row = game->cub_data.map.grid[cell_y];
	if (!row)
		return (true);
	row_len = strlen(row);
	if (cell_x >= row_len)
		return (true);
	cell = row[cell_x];
	if (cell == '1' || cell == '2' || cell == ORB_GHOST_BLOCK_CELL
		|| cell == ' ')
		return (true);
	if (cell >= 'A' && cell <= 'Z' && cell != 'N'
		&& cell != 'S' && cell != 'E' && cell != 'W')
		return (true);
	return (false);
}

bool	collides_with_wall_bonus(t_game *game, float x, float y)
{
	const float	r = game->player_radius;
	float		sx[4];
	float		sy[4];
	int			i;

	if (!game)
		return (true);
	sx[0] = x - r;
	sy[0] = y - r;
	sx[1] = x + r;
	sy[1] = y - r;
	sx[2] = x - r;
	sy[2] = y + r;
	sx[3] = x + r;
	sy[3] = y + r;
	i = 0;
	while (i < 4)
	{
		if (is_cell_blocking_bonus(game, (int)floorf(sx[i]),
				(int)floorf(sy[i])))
			return (true);
		i++;
	}
	return (false);
}

void	attempt_move_bonus(t_game *game, float step_x, float step_y)
{
	float	nx;
	float	ny;

	if (!game)
		return ;
	nx = game->cub_data.player.x + step_x;
	ny = game->cub_data.player.y + step_y;
	if (!collides_with_wall_bonus(game, nx, game->cub_data.player.y))
		game->cub_data.player.x = nx;
	if (!collides_with_wall_bonus(game, game->cub_data.player.x, ny))
		game->cub_data.player.y = ny;
}
