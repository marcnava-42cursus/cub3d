/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 01:46:07 by ivmirand          #+#    #+#             */
/*   Updated: 2025/10/03 09:06:57 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

void	minimap_init(t_game *game)
{
	game->minimap.map = &game->cub_data.map;
	game->minimap.player = &game->cub_data.player;
	game->minimap.bg = mlx_new_image(game->mlx, MINIMAP_WIDTH, MINIMAP_HEIGHT);
	game->minimap.player_vision = mlx_new_image(game->mlx, MINIMAP_WIDTH,
			MINIMAP_HEIGHT);
	game->minimap.player_sprite = mlx_new_image(game->mlx, MINIMAP_PLAYER_SIZE,
			MINIMAP_PLAYER_SIZE);
	game->minimap.tile = mlx_new_image(game->mlx, MINIMAP_TILE_SIZE,
			MINIMAP_TILE_SIZE);
	mlx_image_to_window(game->mlx, game->minimap.bg, MINIMAP_WNDW_X, MINIMAP_WNDW_Y);
	mlx_image_to_window(game->mlx, game->minimap.player_vision, MINIMAP_WNDW_X, MINIMAP_WNDW_Y);
	mlx_image_to_window(game->mlx, game->minimap.player_sprite, MINIMAP_CNTR_X, MINIMAP_CNTR_Y);
	mlx_image_to_window(game->mlx, game->minimap.tile, MINIMAP_WNDW_X, MINIMAP_WNDW_Y);
}

void	render_minimap_bg(t_minimap *minimap)
{
	unsigned int	x;
	unsigned int	y;

	y = 0;
	while (y < minimap->bg->height)
	{
		x = 0;
		while (x < minimap->bg->width)
		{
			save_pixel_to_image(minimap->bg, x, y, BLUE);
			x++;
		}
		y++;
	}
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
			save_pixel_to_image(tile, x, y, color);
			x++;
		}
		y++;
	}
}

void	render_minimap_tiles(t_game *game, t_map *map, t_minimap *minimap)
{
	int		x;
	int		y;
	int		minimap_draw_x;
	int		minimap_draw_y;
	int		minimap_col;
	int		minimap_row;
	size_t	row_len;

	render_minimap_tile(minimap->tile, MINIMAP_TILE_SIZE, WHITE);
	minimap_row = 0;
	minimap_draw_y = -MINIMAP_RADIUS;
	while (minimap_draw_y <= MINIMAP_RADIUS)
	{
		y = minimap->player->y + minimap_draw_y;
		minimap_col = 0;
		if (y >= 0 && y < map->height && map->grid[y])
		{
			minimap_col = 0;
			minimap_draw_x = -MINIMAP_RADIUS;
			row_len = strlen(map->grid[y]);
			while (minimap_draw_x <= MINIMAP_RADIUS)
			{
				x = minimap->player->x + minimap_draw_x;
				if (x >= 0 && (size_t)x < row_len && map->grid[y][x] == '1')
					mlx_image_to_window(game->mlx, minimap->tile,
						MINIMAP_WNDW_X + minimap_col * MINIMAP_TILE_SIZE,
						MINIMAP_WNDW_Y + minimap_row * MINIMAP_TILE_SIZE);
				minimap_draw_x++;
				minimap_col++;
			}
		}
		minimap_draw_y++;
		minimap_row++;
	}
}

void	render_minimap_player(t_minimap *minimap)
{
	render_minimap_tile(minimap->player_sprite, MINIMAP_PLAYER_SIZE, YELLOW);
}

static vertex_t	world_to_minimap_vertex(t_minimap *minimap, vertex_t world)
{
	vertex_t	viewport_origin;
	vertex_t	minimap_vertex;
	vertex_t	player_position;
	float		scale;	

	player_position.x = (minimap->player->x + 0.5f) * WORLDMAP_TILE_SIZE;
	player_position.y = (minimap->player->y + 0.5f) * WORLDMAP_TILE_SIZE;
	viewport_origin.x = player_position.x - (MINIMAP_RADIUS + 0.5f)
		* WORLDMAP_TILE_SIZE;
	viewport_origin.y = player_position.y - (MINIMAP_RADIUS + 0.5f)
		* WORLDMAP_TILE_SIZE;
	scale = (float)MINIMAP_TILE_SIZE / WORLDMAP_TILE_SIZE;
	minimap_vertex.x = (world.x - viewport_origin.x) * scale;
	minimap_vertex.y = (world.y - viewport_origin.y) * scale;
	return (minimap_vertex);
}

void	render_minimap_player_vision(t_minimap *minimap)
{
	const float MAX_DIST = 2000.0f;
	t_rayhit	rayhit;
	vertex_t	player_position;
	vertex_t	tip;
	vertex_t	world_to_minimap;
	vertex_t	player_to_minimap;
	int			i;
	float		ray_angle;
	float		angle_start;
	float		increment;

	player_position.x = (minimap->player->x + 0.5f) * WORLDMAP_TILE_SIZE;
	player_position.y = (minimap->player->y + 0.5f) * WORLDMAP_TILE_SIZE;
	player_to_minimap = world_to_minimap_vertex(minimap, player_position);
	i = 0;
	angle_start = minimap->player->angle - 0.5f * PLAYER_FOV;
	increment = PLAYER_FOV / (float)(MINIMAP_RAYS - 1);
	while (i < MINIMAP_RAYS)
	{
		ray_angle = normalize_angle(angle_start + i * increment);
		rayhit = raycast_world(minimap->map, player_position, ray_angle, MAX_DIST);
		world_to_minimap = world_to_minimap_vertex(minimap, rayhit.position);
		bresenham(&player_to_minimap, &world_to_minimap, minimap->player_vision, GREEN);
		i++;
	}
	tip.x = player_position.x + 100 * cosf(minimap->player->angle);
	tip.y = player_position.y + 100 * sinf(minimap->player->angle); 
	world_to_minimap = world_to_minimap_vertex(minimap, tip);
	bresenham(&player_to_minimap, &world_to_minimap, minimap->player_vision, RED);
}

void	minimap_free(mlx_t *mlx, t_minimap *minimap)
{
	mlx_delete_image(mlx, minimap->tile);
	mlx_delete_image(mlx, minimap->player_vision);
	mlx_delete_image(mlx, minimap->player_sprite);
	mlx_delete_image(mlx, minimap->bg);
}
