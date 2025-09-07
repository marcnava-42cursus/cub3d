/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 01:46:07 by ivmirand          #+#    #+#             */
/*   Updated: 2025/09/07 02:21:19 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

void	minimap_init(t_game *game)
{
	game->minimap.map = &game->cub_data.map;
	game->minimap.player = &game->cub_data.player;
	game->minimap.tile_size = MINIMAP_TILE_SIZE;
	game->minimap.bg = mlx_new_image(game->mlx, MINIMAP_WIDTH, MINIMAP_HEIGHT);
	game->minimap.tile = mlx_new_image(game->mlx, game->minimap.tile_size,
			game->minimap.tile_size);
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

void	render_minimap_player(mlx_t *mlx, t_minimap *minimap)
{
	unsigned int	x;
	unsigned int	y;

	y = 0;
	while (y < minimap->tile_size)
	{
		x = 0;
		while (x < minimap->tile_size)
		{
			mlx_put_pixel(minimap->tile, x, y, YELLOW);	
			x++;
		}
		y++;
	}
	mlx_image_to_window(mlx, minimap->tile,
			MAX_WINDOW_WIDTH - (MINIMAP_WIDTH / 2),
			MAX_WINDOW_HEIGHT - (MINIMAP_HEIGHT / 2));
}

void	minimap_free(mlx_t *mlx, t_minimap *minimap)
{
	mlx_delete_image(mlx, minimap->tile);
	mlx_delete_image(mlx, minimap->bg);
}
