/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_tile.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 17:47:44 by ivmirand          #+#    #+#             */
/*   Updated: 2025/12/30 20:18:11 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static uint32_t	get_minimap_wall_color(char tile)
{
	if (tile == '1')
		return (WHITE);
	if (tile == '2')
		return (LIGHT_GREY);
	return (0);
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

void	render_minimap_walls(t_minimap *minimap)
{
	int		pixel[2];
	float	player_angle_inverse;
	float	cos_pai;
	float	sin_pai;
	float	d[2];
	int		world_pixel[2];
	float	world[2];
	int		tile_color;

	player_angle_inverse = minimap->player->angle + (FT_PI / 2.0f);
	cos_pai = cosf(player_angle_inverse);
	sin_pai = sinf(player_angle_inverse);
	pixel[Y] = 0;
	while (pixel[Y] < (int)minimap->bg->height)
	{
		pixel[X] = 0;
		while (pixel[X] < (int)minimap->bg->width)
		{
			if (is_inside_minimap_circle(pixel))
			{
				d[X] = (float)pixel[X] - (MINIMAP_WIDTH / 2.0f);	
				d[Y] = (float)pixel[Y] - (MINIMAP_HEIGHT / 2.0f);	
				world[X] = d[X] * cos_pai - d[Y] * sin_pai;
				world[Y] = d[X] * sin_pai + d[Y] * cos_pai;
				world[X] = minimap->player->x + (world[X] / (float)MINIMAP_TILE_SIZE);
				world[Y] = minimap->player->y + (world[Y] / (float)MINIMAP_TILE_SIZE);
				world_pixel[X] = (int)floorf(world[X]);
				world_pixel[Y] = (int)floorf(world[Y]);
				if (world_pixel[Y] >= 0 && world_pixel[Y] < minimap->map->height
						&& minimap->map->grid[world_pixel[Y]]
						&& world_pixel[X] >= 0 && world_pixel[X]
							< (int)ft_strlen(minimap->map->grid[world_pixel[Y]]))
				{
					tile_color = get_minimap_wall_color(
							minimap->map->grid[world_pixel[Y]][world_pixel[X]]);						
					if (tile_color != 0)
						save_pixel_to_image(minimap->bg, pixel[X], pixel[Y],
								tile_color);
				}
			}
			pixel[X]++;
		}
		pixel[Y]++;
	}
}

void	render_minimap_player(t_minimap *minimap)
{
	render_minimap_tile(minimap->player_sprite, MINIMAP_PLAYER_SIZE, RED);
}

//void	render_minimap_tiles(t_map *map, t_minimap *minimap)
//{
//	int			pixel[2];
//	int			world[2];
//	int			tile[2];
//	vertex_t	rotate;
//	vertex_t	scrn_pos;
//	int			tile_color;
//
//	// Render tiles by sampling world space in a rotated grid
//	world[Y] = minimap->player->y - MINIMAP_TILE_RADIUS;
//	while (world[Y] <= minimap->player->y + MINIMAP_TILE_RADIUS)
//	{
//		if (world[Y] < 0 || world[Y] >= map->height || !map->grid[world[Y]])
//		{
//			world[Y]++;
//			continue ;
//		}
//		world[X] = minimap->player->x - MINIMAP_TILE_RADIUS;
//		while (world[X] <= minimap->player->x + MINIMAP_TILE_RADIUS)
//		{
//			if (world[X] < 0 || world[X] >= (int)ft_strlen(map->grid[world[Y]]))
//			{
//				world[X]++;
//				continue ;
//			}
//			tile_color = get_minimap_wall_color(map->grid[world[Y]][world[X]]);
//			if (tile_color == 0)
//			{
//				world[X]++;
//				continue ;
//			}
//			rotate = rotate_point(
//					(world[X] - minimap->player->x + 0.5f) * MINIMAP_TILE_SIZE,
//					(world[Y] - minimap->player->y + 0.5f) * MINIMAP_TILE_SIZE,
//					-(minimap->player->angle + FT_PI / 2));
//			// Convert to screen position (subtract half tile size to center the tile)
//			scrn_pos.x = MINIMAP_WIDTH / 2 + rotate.x - MINIMAP_TILE_SIZE / 2;
//			scrn_pos.y = MINIMAP_HEIGHT / 2 + rotate.y - MINIMAP_TILE_SIZE / 2;
//			// Draw the tile (as a filled rectangle) if it's inside the circle
//			tile[Y] = 0;
//			while (tile[Y] < MINIMAP_TILE_SIZE)
//			{
//				tile[X] = 0;
//				while (tile[X] < MINIMAP_TILE_SIZE)
//				{
//					pixel[X] = (int)(scrn_pos.x + tile[X]);
//					pixel[Y] = (int)(scrn_pos.y + tile[Y]);
//					if (pixel[X] >= 0 && pixel[X] < (int)minimap->bg->width
//						&& pixel[Y] >= 0 && pixel[Y] < (int)minimap->bg->height
//						&& is_inside_minimap_circle(pixel))
//						save_pixel_to_image(minimap->bg, pixel[X], pixel[Y],
//							tile_color);
//					tile[X]++;
//				}
//				tile[Y]++;
//			}
//			world[X]++;
//		}
//		world[Y]++;
//	}
//}

