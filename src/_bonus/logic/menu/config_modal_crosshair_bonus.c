/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_modal_crosshair_bonus.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/27 00:00:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logic_bonus.h"
#include "render.h"
#include "structs.h"

/**
 * @brief Draws a crosshair in the center of the screen
 *
 * @param game Game state structure
 */
static void	draw_crosshair(t_game *game)
{
	int			center_x;
	int			center_y;
	int			i;
	int			thickness;
	uint32_t	color;

	if (!game || !game->crosshair)
		return ;
	center_x = game->crosshair->width / 2;
	center_y = game->crosshair->height / 2;
	color = 0x000000FF;
	i = -20;
	while (i <= 20)
	{
		if (i >= -3 && i <= 3)
		{
			i++;
			continue ;
		}
		thickness = -1;
		while (thickness <= 1)
		{
			mlx_put_pixel(game->crosshair, center_x + i,
				center_y + thickness, color);
			mlx_put_pixel(game->crosshair, center_x + thickness,
				center_y + i, color);
			thickness++;
		}
		i++;
	}
}

/**
 * @brief Initializes the crosshair overlay
 *
 * Creates a transparent image and draws a green crosshair in the center
 *
 * @param game Game state structure
 */
void	init_crosshair(t_game *game)
{
	uint32_t	i;
	uint32_t	total_pixels;

	if (!game || !game->mlx)
		return ;
	game->crosshair = mlx_new_image(game->mlx, MAX_WINDOW_WIDTH,
			MAX_WINDOW_HEIGHT);
	if (!game->crosshair)
		return ;
	total_pixels = game->crosshair->width * game->crosshair->height;
	i = 0;
	while (i < total_pixels)
	{
		((uint32_t *)game->crosshair->pixels)[i] = 0x00000000;
		i++;
	}
	draw_crosshair(game);
	mlx_image_to_window(game->mlx, game->crosshair, 0, 0);
}
