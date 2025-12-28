/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 01:46:07 by ivmirand          #+#    #+#             */
/*   Updated: 2025/12/28 20:09:55 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

void	minimap_init(t_game *game)
{
	game->minimap.map = &game->cub_data.map;
	game->minimap.player = &game->cub_data.player;
	game->minimap.bg = mlx_new_image(game->mlx, MINIMAP_WIDTH, MINIMAP_HEIGHT);
	game->minimap.player_sprite = mlx_new_image(game->mlx, MINIMAP_PLAYER_SIZE,
			MINIMAP_PLAYER_SIZE);
	game->minimap.player_vision = mlx_new_image(game->mlx, MINIMAP_WIDTH,
			MINIMAP_HEIGHT);
	game->minimap.tile = mlx_new_image(game->mlx, MINIMAP_TILE_SIZE,
			MINIMAP_TILE_SIZE);
	mlx_image_to_window(game->mlx, game->minimap.bg, MINIMAP_WNDW_X,
		MINIMAP_WNDW_Y);
	mlx_image_to_window(game->mlx, game->minimap.player_vision,
		MINIMAP_WNDW_X, MINIMAP_WNDW_Y);
	mlx_image_to_window(game->mlx, game->minimap.player_sprite,
		MINIMAP_CNTR_X, MINIMAP_CNTR_Y);
}

void	render_minimap_bg(t_minimap *minimap)
{
	int	pixel[2];

	ft_bzero(minimap->bg->pixels, minimap->bg->width * minimap->bg->height * 4);
	pixel[Y] = 0;
	while (pixel[Y] < minimap->bg->height)
	{
		pixel[X] = 0;
		while (pixel[X] < minimap->bg->width)
		{
			if (is_inside_minimap_circle(pixel))
				save_pixel_to_image(minimap->bg, pixel[X], pixel[Y], BLUE);
			pixel[X]++;
		}
		pixel[Y]++;
	}
}

void	minimap_free(mlx_t *mlx, t_minimap *minimap)
{
	mlx_delete_image(mlx, minimap->tile);
	mlx_delete_image(mlx, minimap->player_vision);
	mlx_delete_image(mlx, minimap->player_sprite);
	mlx_delete_image(mlx, minimap->bg);
}
