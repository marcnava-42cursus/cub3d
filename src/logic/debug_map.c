/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 12:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/19 20:13:49 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logic.h"
#include "render.h"
#include "structs.h"

/**
 * @brief Gets the character representing player direction based on angle
 *
 * @param angle Player's current angle in radians
 * @return char Symbol representing the direction (>, v, <, ^)
 */
static char	get_player_direction_char(float angle)
{
	if (angle >= ANGLE_RIGHT_MIN && angle < ANGLE_RIGHT_MAX)
		return (DIR_RIGHT);
	else if (angle >= ANGLE_DOWN_MIN && angle < ANGLE_DOWN_MAX)
		return (DIR_DOWN);
	else if (angle >= ANGLE_LEFT_MIN || angle < ANGLE_LEFT_MAX)
		return (DIR_LEFT);
	else
		return (DIR_UP);
}

/**
 * @brief Prints player information header
 *
 * @param game Game state structure
 * @param grid_x Current grid X position
 * @param grid_y Current grid Y position
 */
static void	print_player_info(t_game *game, int grid_x, int grid_y)
{
	if (!game)
		return ;
	printf("Player position: (%.2f, %.2f) - Grid: (%d, %d) - Angle: %.2f\n",
		game->cub_data.player.x, game->cub_data.player.y,
		grid_x, grid_y, game->cub_data.player.angle);
}

/**
 * @brief Prints a single map cell (either player or map tile)
 *
 * @param game Game state structure
 * @param x Current X coordinate
 * @param y Current Y coordinate
 * @param player_pos Array containing player's grid position [x, y]
 */
static void	print_map_cell(t_game *game, int x, int y, int player_pos[2])
{
	if (x == player_pos[0] && y == player_pos[1])
		printf("%c", get_player_direction_char(game->cub_data.player.angle));
	else
		printf("%c", game->cub_data.map.grid[y][x]);
}

/**
 * @brief Prints the 2D map grid with player position
 *
 * @param game Game state structure
 * @param player_pos Array containing player's grid position [x, y]
 */
static void	print_map_grid(t_game *game, int player_pos[2])
{
	int	y;
	int	x;

	if (!game || !game->cub_data.map.grid)
		return ;
	y = 0;
	while (y < game->cub_data.map.height)
	{
		x = 0;
		while (x < game->cub_data.map.width)
		{
			print_map_cell(game, x, y, player_pos);
			x++;
		}
		printf("\n");
		y++;
	}
}

/**
 * @brief Prints controls information
 */
void	print_controls(void)
{
	printf("\n=== CONTROLS ===\n");
	printf("WASD: Movement\n");
	printf("Arrow Keys: Rotation\n");
	printf("M: Toggle minimap overlay\n");
	printf("ESC: Exit\n\n");
}

/**
 * @brief Prints the complete 2D map in terminal with player position
 *
 * This function clears the screen and displays the current map state
 * with the player's position and orientation.
 *
 * @param game Pointer to the game structure
 */
void	print_map_2d(t_game *game)
{
	int	player_pos[2];

	if (!game)
		return ;
	player_pos[0] = (int)floor(game->cub_data.player.x);
	player_pos[1] = (int)floor(game->cub_data.player.y);
	printf(ANSI_CLEAR_SCREEN);
	print_player_info(game, player_pos[0], player_pos[1]);
	printf("Map:\n");
	print_map_grid(game, player_pos);
	printf("\n");
}

/**
 * @brief Gets the character at a specific map cell
 *
 * @param map Map structure
 * @param x Cell X coordinate
 * @param y Cell Y coordinate
 * @return char The character at that cell or '?' if invalid
 */
static char	get_cell_char(const t_map *map, int x, int y)
{
	const char	*row;
	int			len;

	if (!map || !map->grid || y < 0 || y >= map->height)
		return ('?');
	row = map->grid[y];
	if (!row)
		return ('?');
	len = (int)ft_strlen(row);
	if (x < 0 || x >= len)
		return ('?');
	return (row[x]);
}

/**
 * @brief Prints inline player position update (without full map)
 *
 * @param game Game state structure
 * @param grid_x Current grid X position
 * @param grid_y Current grid Y position
 */
void	print_player_position_inline(t_game *game, int grid_x, int grid_y)
{
	t_rayhit	hit;
	vertex_t	start;
	char		block_char;

	if (!game)
		return ;
	start.x = game->cub_data.player.x * WORLDMAP_TILE_SIZE;
	start.y = game->cub_data.player.y * WORLDMAP_TILE_SIZE;
	hit = raycast_world(&game->cub_data.map, start,
			game->cub_data.player.angle, 1000.0f);
	if (hit.hit)
	{
		block_char = get_cell_char(&game->cub_data.map,
				hit.cell[0], hit.cell[1]);
		printf("Player: (%.2f, %.2f) - Grid: (%d, %d) - Angle: %.2f - Looking at: %c (%d, %d)\r",
			game->cub_data.player.x, game->cub_data.player.y,
			grid_x, grid_y, game->cub_data.player.angle,
			block_char, hit.cell[0], hit.cell[1]);
	}
	else
	{
		printf("Player: (%.2f, %.2f) - Grid: (%d, %d) - Angle: %.2f - Looking at: nothing\r",
			game->cub_data.player.x, game->cub_data.player.y,
			grid_x, grid_y, game->cub_data.player.angle);
	}
	fflush(stdout);
}

/**
 * @brief Checks if player changed grid cell and prints map if needed
 *
 * @param game Game state structure
 * @param grid_x Current grid X position
 * @param grid_y Current grid Y position
 */
void	handle_debug_map_update(t_game *game, int grid_x, int grid_y)
{
	if (!game)
		return ;
	if (grid_x != game->last_grid_x || grid_y != game->last_grid_y)
	{
		print_map_2d(game);
		game->last_grid_x = grid_x;
		game->last_grid_y = grid_y;
	}
	else
	{
		print_player_position_inline(game, grid_x, grid_y);
	}
}
