/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   background.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 12:12:46 by ivmirand          #+#    #+#             */
/*   Updated: 2025/09/25 14:07:43 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static void	render_bg_color(t_game *game)
{
	unsigned int	x;
	unsigned int	y;
	int				sky;
	int				ground;

	y = 0;
	sky = t_color_to_int(&game->cub_data.ceiling_color, 255);
	ground = t_color_to_int(&game->cub_data.floor_color, 255);
	while (y < game->render_buf[BACKGROUND]->height / 2)
	{
		x = 0;
		while (x < game->render_buf[BACKGROUND]->width)
		{
			save_pixel_to_image(game->render_buf[BACKGROUND], x, y, sky);
			x++;
		}
		y++;
	}
	while (y < game->render_buf[BACKGROUND]->height)
	{
		x = 0;
		while (x < game->render_buf[BACKGROUND]->width)
		{
			save_pixel_to_image(game->render_buf[BACKGROUND], x, y, ground);
			x++;
		}
		y++;
	}
	mlx_image_to_window(game->mlx, game->render_buf[BACKGROUND], 0, 0);
}

static void	render_atmospheric_perspective(t_game *game)
{
	unsigned int	x;
	unsigned int	y;
	mlx_image_t	*atmos_buf;

	atmos_buf = game->render_buf[ATMOSPHERE];
	y = atmos_buf->height / 6;
	while (y < atmos_buf->height / 3)
	{
		x = 0;
		while (x < atmos_buf->width)
		{
			if (x % 8 == 0 && y % 8 == 0)
				save_pixel_to_image(atmos_buf, x, y, WHITE);
			x++;
		}
		y++;
	}
	while (y < atmos_buf->height / 2)
	{
		x = 0;
		while (x < atmos_buf->width)
		{
			if (x % 4 == 0 && y % 4 == 0)
				save_pixel_to_image(atmos_buf, x, y, WHITE);
			x++;
		}
		y++;
	}
	while (y < atmos_buf->height - (atmos_buf->height / 3))
		y++;
	while (y < atmos_buf->height - (atmos_buf->height / 6))
	{
		x = 0;
		while (x < atmos_buf->width)
		{
			if (x % 3 == 0 && y % 3 == 0)
				save_pixel_to_image(atmos_buf, x, y, BLACK);
			x++;
		}
		y++;
	}
	while (y < atmos_buf->height)
	{
		x = 0;
		while (x < atmos_buf->width)
		{
			if (x % 2 == 0 && y % 2 == 0)
				save_pixel_to_image(atmos_buf, x, y, BLACK);
			x++;
		}
		y++;
	}
	mlx_image_to_window(game->mlx, atmos_buf, 0, 0);
}

void	render_bg(t_game *game)
{
	render_bg_color(game);
	render_atmospheric_perspective(game);
}
