/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_tile.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 17:47:44 by ivmirand          #+#    #+#             */
/*   Updated: 2025/12/28 10:26:56 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static bool	is_minimap_wall(char tile)
{
	return (tile == '1' || tile == '2');
}

static uint32_t	get_minimap_wall_color(char tile)
{
	if (tile == '2')
		return (ORANGE);
	return (WHITE);
}

static void	render_minimap_tile(mlx_image_t *tile, int tile_size, int color)
{
	unsigned int	x;
	unsigned int	y;

	y = 0;
	while (y < (unsigned int)tile_size)
	{
		x = 0;
		while (x < (unsigned int)tile_size)
		{
			save_pixel_to_image(tile, x, y, color);
			x++;
		}
		y++;
	}
}

void	render_minimap_tiles(t_map *map, t_minimap *minimap)
{
	int			pixel_x;
	int			pixel_y;
	vertex_t	world_offset;
	vertex_t	rotate;
	vertex_t	screen_pos;
	size_t		row_len;
	char		tile;
	uint32_t	tile_color;

	// Render tiles by sampling world space in a rotated grid
	for (int world_y = minimap->player->y - MINIMAP_TILE_RADIUS; world_y <= minimap->player->y + MINIMAP_TILE_RADIUS; world_y++)
	{
		if (world_y < 0 || world_y >= map->height || !map->grid[world_y])
			continue ;
		row_len = ft_strlen(map->grid[world_y]);
		for (int world_x = minimap->player->x - MINIMAP_TILE_RADIUS; world_x <= minimap->player->x + MINIMAP_TILE_RADIUS; world_x++)
		{
			if (world_x < 0 || (size_t)world_x >= row_len)
				continue ;
			tile = map->grid[world_y][world_x];
			if (!is_minimap_wall(tile))
				continue ;
			tile_color = get_minimap_wall_color(tile);
			// Calculate offset from tile center to tile center
			world_offset.x = (world_x - minimap->player->x + 0.5f)
				* MINIMAP_TILE_SIZE;
			world_offset.y = (world_y - minimap->player->y + 0.5f)
				* MINIMAP_TILE_SIZE;
			rotate = rotate_point(world_offset,
					-(minimap->player->angle + FT_PI / 2));
			// Convert to screen position (subtract half tile size to center the tile)
			screen_pos.x = MINIMAP_WIDTH / 2 + rotate.x - MINIMAP_TILE_SIZE / 2;
			screen_pos.y = MINIMAP_HEIGHT / 2 + rotate.y - MINIMAP_TILE_SIZE / 2;
			// Draw the tile (as a filled rectangle) if it's inside the circle
			for (int tile_y = 0; tile_y < MINIMAP_TILE_SIZE; tile_y++)
			{
				for (int tile_x = 0; tile_x < MINIMAP_TILE_SIZE; tile_x++)
				{
					pixel_x = (int)(screen_pos.x + tile_x);
					pixel_y = (int)(screen_pos.y + tile_y);
					if (pixel_x >= 0 && pixel_x < (int)minimap->bg->width &&
						pixel_y >= 0 && pixel_y < (int)minimap->bg->height &&
						is_inside_minimap_circle(pixel_x, pixel_y))
						save_pixel_to_image(minimap->bg, pixel_x, pixel_y,
							tile_color);
				}
			}
		}
	}
}

void	render_minimap_player(t_minimap *minimap)
{
	render_minimap_tile(minimap->player_sprite, MINIMAP_PLAYER_SIZE, YELLOW);
}
