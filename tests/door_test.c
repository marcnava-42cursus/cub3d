/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 20:15:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/11/12 20:15:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "logic.h"
#include "render.h"

/**
 * @brief Changes wall ('1') to floor ('0') at specific coordinates
 *
 * @param game Game state structure
 * @param x X coordinate in grid
 * @param y Y coordinate in grid
 */
static void	break_wall(t_game *game, int x, int y)
{
	char	*cell;

	if (!game || !game->cub_data.map.grid)
		return ;
	if (y < 0 || y >= game->cub_data.map.height)
		return ;
	if (x < 0 || x >= (int)ft_strlen(game->cub_data.map.grid[y]))
		return ;
	cell = &game->cub_data.map.grid[y][x];
	if (*cell == '1')
	{
		*cell = '0';
		printf("Wall broken at (%d, %d)\n", x, y);
	}
	else
	{
		printf("Not a wall at (%d, %d), cell is '%c'\n", x, y, *cell);
	}
}

/**
 * @brief Breaks wall in front of player (only if it's a wall '1')
 *
 * This function casts a ray from the player's position in the direction
 * they're facing. If the ray hits a wall ('1'), it changes it to floor ('0').
 * After breaking the wall, it forces a re-render to show the change immediately.
 *
 * @param game Game state structure
 */
void	test_break_wall_in_front(t_game *game)
{
	t_rayhit	hit;
	vertex_t	start;
	char		cell;
	bool		wall_broken;

	if (!game)
		return ;
	wall_broken = false;
	start.x = game->cub_data.player.x * WORLDMAP_TILE_SIZE;
	start.y = game->cub_data.player.y * WORLDMAP_TILE_SIZE;
	hit = raycast_world(&game->cub_data.map, start,
			game->cub_data.player.angle, 300.0f);
	if (hit.hit)
	{
		cell = game->cub_data.map.grid[hit.cell_y][hit.cell_x];
		printf("Looking at cell (%d, %d) with character '%c'\n",
			hit.cell_x, hit.cell_y, cell);
		if (cell == '1')
		{
			break_wall(game, hit.cell_x, hit.cell_y);
			wall_broken = true;
		}
		else
			printf("Cannot break: not a wall\n");
	}
	else
	{
		printf("No wall in front\n");
	}
	if (wall_broken)
		render_double_buffer(game);
}
