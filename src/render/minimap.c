/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 01:46:07 by ivmirand          #+#    #+#             */
/*   Updated: 2025/09/11 13:00:05 by ivmirand         ###   ########.fr       */
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
	game->minimap.player_vision = mlx_new_image(game->mlx,
			MINIMAP_WIDTH, MINIMAP_HEIGHT);
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
	int		x;
	int		y;
	int		minimap_draw_x;
	int		minimap_draw_y;
	int		minimap_origin_x;
	int		minimap_origin_y;
	int		minimap_col;
	int		minimap_row;
	size_t	row_len;

	minimap_origin_x = MAX_WINDOW_WIDTH - MINIMAP_WIDTH;
	minimap_origin_y = MAX_WINDOW_HEIGHT - MINIMAP_HEIGHT;
	render_minimap_tile(minimap->tile, MINIMAP_TILE_SIZE, WHITE);
	minimap_row = 0;
	minimap_draw_y = -MINIMAP_RADIUS;
	while (minimap_draw_y <= MINIMAP_RADIUS)
	{
		y = minimap->player->y + minimap_draw_y;
		minimap_col = 0;
		if (y >= 0 && y < map->height)
		{
			minimap_col = 0;
			minimap_draw_x = -MINIMAP_RADIUS;
			while (minimap_draw_x <= MINIMAP_RADIUS)
			{
				row_len = strlen(map->grid[y]);
				x = minimap->player->x + minimap_draw_x;
				if (x >= 0 && (size_t)x < row_len && map->grid[y][x] == '1') 
					mlx_image_to_window(mlx, minimap->tile,
						minimap_origin_x + minimap_col * MINIMAP_TILE_SIZE,
						minimap_origin_y + minimap_row * MINIMAP_TILE_SIZE);
				minimap_draw_x++;
				minimap_col++;
			}
		}
		minimap_draw_y++;
		minimap_row++;
	}
}

void	render_minimap_player(mlx_t *mlx, t_minimap *minimap)
{
	int	minimap_origin_x;
	int	minimap_origin_y;
	int	minimap_center_x;
	int	minimap_center_y;

	minimap_origin_x = MAX_WINDOW_WIDTH - MINIMAP_WIDTH;
	minimap_origin_y = MAX_WINDOW_HEIGHT - MINIMAP_HEIGHT;
	minimap_center_x = minimap_origin_x + MINIMAP_RADIUS * MINIMAP_TILE_SIZE +
		(MINIMAP_TILE_SIZE - MINIMAP_PLAYER_SIZE) / 2;
	minimap_center_y = minimap_origin_y + MINIMAP_RADIUS * MINIMAP_TILE_SIZE +
		(MINIMAP_TILE_SIZE - MINIMAP_PLAYER_SIZE) / 2;
	render_minimap_tile(minimap->player_sprite, MINIMAP_PLAYER_SIZE, YELLOW);
	mlx_image_to_window(mlx, minimap->player_sprite, minimap_center_x,
			minimap_center_y);
}

void	render_minimap_player_vision(mlx_t *mlx, t_minimap *minimap)
{
	int	minimap_origin_x;
	int	minimap_origin_y;
	int	minimap_center_x;
	int	minimap_center_y;

	minimap_origin_x = MAX_WINDOW_WIDTH - MINIMAP_WIDTH;
	minimap_origin_y = MAX_WINDOW_HEIGHT - MINIMAP_HEIGHT;
	minimap_center_x = minimap_origin_x + MINIMAP_RADIUS * MINIMAP_TILE_SIZE +
		(MINIMAP_TILE_SIZE - MINIMAP_PLAYER_SIZE) / 2;
	minimap_center_y = minimap_origin_y + MINIMAP_RADIUS * MINIMAP_TILE_SIZE +
		(MINIMAP_TILE_SIZE - MINIMAP_PLAYER_SIZE) / 2;
	//calculate fov
	mlx_image_to_window(mlx, minimap->player_vision, minimap_center_x,
			minimap_center_y);
}

void	minimap_free(mlx_t *mlx, t_minimap *minimap)
{
	mlx_delete_image(mlx, minimap->tile);
	mlx_delete_image(mlx, minimap->player_vision);
	mlx_delete_image(mlx, minimap->player_sprite);
	mlx_delete_image(mlx, minimap->bg);
}
