/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_swapper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 20:15:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/12/30 19:37:05 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "logic.h"
#include "render.h"

/**
 * @brief Apply interaction to the targeted cell.
 * Returns true when the cell changes state.
 */
static bool	modify_interactive_cell(t_game *game, int x, int y)
{
	char	*cell;

	if (!game || !game->cub_data.map.grid)
		return (false);
	if (y < 0 || y >= game->cub_data.map.height)
		return (false);
	if (x < 0 || x >= (int)ft_strlen(game->cub_data.map.grid[y]))
		return (false);
	cell = &game->cub_data.map.grid[y][x];
	if (*cell == '2')
	{
		if (!store_block_in_inventory(&game->cub_data.player, *cell))
		{
			printf("Cannot break block: inventory already contains one\n");
			return (false);
		}
		*cell = '0';
		return (true);
	}
	if (*cell == 'D')
	{
		*cell = 'd';
		return (true);
	}
	if (*cell == 'd')
	{
		*cell = 'D';
		return (true);
	}
	return (false);
}

static bool	is_valid_target_cell(t_game *game, int x, int y)
{
	int		player_x;
	int		player_y;
	size_t	row_len;

	if (!game || !game->cub_data.map.grid)
		return (false);
	if (y < 0 || y >= game->cub_data.map.height)
		return (false);
	row_len = ft_strlen(game->cub_data.map.grid[y]);
	if (x < 0 || x >= (int)row_len)
		return (false);
	player_x = (int)floorf(game->cub_data.player.x);
	player_y = (int)floorf(game->cub_data.player.y);
	if (x == player_x && y == player_y)
		return (false);
	return (game->cub_data.map.grid[y][x] == '0');
}

static bool	get_attachment_coords(t_game *game, t_rayhit *hit, int *x, int *y)
{
	vertex_t	dir;
	vertex_t	point_before_hit;

	if (!game || !hit || !hit->hit)
		return (false);
	dir.x = cosf(game->cub_data.player.angle);
	dir.y = sinf(game->cub_data.player.angle);
	point_before_hit.x = hit->position.x - dir.x;
	point_before_hit.y = hit->position.y - dir.y;
	*x = (int)floorf(point_before_hit.x / WORLDMAP_TILE_SIZE);
	*y = (int)floorf(point_before_hit.y / WORLDMAP_TILE_SIZE);
	return (true);
}

/**
 * @brief Places a breakable block ('2') on the face of the wall the player
 * is looking at, if the adjacent cell is empty floor.
 */
void	place_breakable_block(t_game *game)
{
	t_rayhit	hit;
	vertex_t	start;
	char		cell;
	int			target_x;
	int			target_y;

	if (!game)
		return ;
	if (!player_has_block(&game->cub_data.player))
	{
		printf("Inventory is empty, cannot place a block\n");
		return ;
	}
	start.x = (game->cub_data.player.x) * WORLDMAP_TILE_SIZE;
	start.y = (game->cub_data.player.y) * WORLDMAP_TILE_SIZE;
	hit = raycast_world(&game->cub_data.map, start,
			game->cub_data.player.angle, 300.0f);
	if (!hit.hit)
	{
		printf("No wall in front to attach block\n");
		return ;
	}
	if (hit.cell[1] < 0 || hit.cell[1] >= game->cub_data.map.height
		|| hit.cell[0] < 0
		|| hit.cell[0] >= (int)ft_strlen(game->cub_data.map.grid[hit.cell[1]]))
		return ;
	cell = game->cub_data.map.grid[hit.cell[1]][hit.cell[0]];
	if (cell != '1' && cell != '2')
	{
		printf("Cannot place block on cell '%c'\n", cell);
		return ;
	}
	if (!get_attachment_coords(game, &hit, &target_x, &target_y))
		return ;
	if (!is_valid_target_cell(game, target_x, target_y))
	{
		printf("Cannot place block at (%d, %d)\n", target_x, target_y);
		return ;
	}
	cell = consume_inventory_block(&game->cub_data.player);
	if (cell == '\0')
		return ;
	game->cub_data.map.grid[target_y][target_x] = cell;
	printf("Placed breakable block at (%d, %d)\n", target_x, target_y);
	render_double_buffer(game);
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
	bool		cell_changed;

	if (!game)
		return ;
	cell_changed = false;
	start.x = (game->cub_data.player.x) * WORLDMAP_TILE_SIZE;
	start.y = (game->cub_data.player.y) * WORLDMAP_TILE_SIZE;
	hit = raycast_world(&game->cub_data.map, start,
			game->cub_data.player.angle, 300.0f);
	if (hit.hit)
	{
		cell = game->cub_data.map.grid[hit.cell[1]][hit.cell[0]];
		printf("Looking at cell (%d, %d) with character '%c'\n",
			hit.cell[0], hit.cell[1], cell);
		if (cell == '2')
		{
			cell_changed = modify_interactive_cell(game,
					hit.cell[0], hit.cell[1]);
			if (cell_changed)
				printf("Breakable block stored in inventory!\n");
		}
		else if (cell == 'D' || cell == 'd')
			cell_changed = modify_interactive_cell(game, hit.cell[0], hit.cell[1]);
		else
			printf("Cannot break cell '%c'\n", cell);
	}
	else
		printf("No wall in front\n");
	if (cell_changed)
		render_double_buffer(game);
}
