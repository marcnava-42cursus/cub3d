/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 01:46:07 by ivmirand          #+#    #+#             */
/*   Updated: 2025/09/08 12:20:01 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

void	minimap_init(t_game *game)
{
	game->minimap.map = &game->cub_data.map;
	game->minimap.player = &game->cub_data.player;
	game->minimap.bg = mlx_new_image(game->mlx, MINIMAP_WIDTH, MINIMAP_HEIGHT);
	game->minimap.player_sprite = mlx_new_image(game->mlx,
			MINIMAP_PLAYER_SIZE, MINIMAP_PLAYER_SIZE);
	game->minimap.tile = mlx_new_image(game->mlx, MINIMAP_TILE_SIZE,
			MINIMAP_TILE_SIZE);
}

void	render_minimap_bg(mlx_t *mlx, t_minimap *minimap)
{
	unsigned int	x;
	unsigned int	y;

	y = 0;
	while (y < minimap->bg->height)
	{
		x = 0;
		while (x < minimap->bg->width)
		{
			mlx_put_pixel(minimap->bg, x, y, BLUE);
			x++;
		}
		y++;
	}
	mlx_image_to_window(mlx, minimap->bg,
			MAX_WINDOW_WIDTH - (minimap->bg->width),
			MAX_WINDOW_HEIGHT - (minimap->bg->height));
}

void	render_minimap_tile(mlx_image_t *tile, int tile_size, int color)
{
	unsigned int	x;
	unsigned int	y;
	
	y = 0;
	while (y < (unsigned int)tile_size)
	{
		x = 0;
		while (x < (unsigned int)tile_size)
		{
			mlx_put_pixel(tile, x, y, color);
			x++;
		}
		y++;
	}
}
void	render_minimap_tiles(mlx_t *mlx, t_map *map, t_minimap *minimap)
{
	int	x;
	int	y;
	int	minimap_origin_x;
	int	minimap_origin_y;
	int	minimap_col;
	int	minimap_row;

	y = minimap->player->y - 8;
	minimap_origin_x = MAX_WINDOW_WIDTH - minimap->bg->width;
	minimap_origin_y = MAX_WINDOW_HEIGHT - minimap->bg->height;
	render_minimap_tile(minimap->tile, MINIMAP_TILE_SIZE, WHITE);
	minimap_row = 0;
	while (y < minimap->player->y + 8)
	{
		x = minimap->player->x - 8;
		minimap_col = 0;
		while (x < minimap->player->x + 8)
		{
			if (y >= 0 && y < map->height && x > 0 && map->grid[y])
			{
				if ((size_t)(x - 1) < strlen(map->grid[y])
					&& map->grid[y][x - 1] == '1') 
					mlx_image_to_window(mlx, minimap->tile,
						minimap_origin_x + minimap_col * MINIMAP_TILE_SIZE,
						minimap_origin_y + minimap_row * MINIMAP_TILE_SIZE);
			}
			x++;
			minimap_col++;
		}
		y++;
		minimap_row++;
	}
}

void	render_minimap_player(mlx_t *mlx, t_minimap *minimap)
{
	render_minimap_tile(minimap->player_sprite, MINIMAP_PLAYER_SIZE, YELLOW);
	mlx_image_to_window(mlx, minimap->player_sprite,
			MAX_WINDOW_WIDTH - (MINIMAP_WIDTH / 2) - 1,
			MAX_WINDOW_HEIGHT - (MINIMAP_HEIGHT / 2) - 1);
}

void	minimap_free(mlx_t *mlx, t_minimap *minimap)
{
	mlx_delete_image(mlx, minimap->tile);
	mlx_delete_image(mlx, minimap->player_sprite);
	mlx_delete_image(mlx, minimap->bg);
}
