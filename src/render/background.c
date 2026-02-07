/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   background.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 12:12:46 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/07 19:28:12 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static int	t_color_to_int(t_color *color, int alpha)
{
	return (color->r << 24 | color->g << 16 | color->b << 8 | alpha);
}

void	render_bg_color(t_game *game, int sky, int ground)
{
	unsigned int	pixel[2];

	pixel[Y] = 0;
	while (pixel[Y] < game->double_buffer[NEXT]->height / 2)
	{
		pixel[X] = 0;
		while (pixel[X] < game->double_buffer[NEXT]->width)
		{
			save_pixel_to_image(game->double_buffer[NEXT], pixel[X], pixel[Y],
				sky);
			pixel[X]++;
		}
		pixel[Y]++;
	}
	while (pixel[Y] < game->double_buffer[NEXT]->height)
	{
		pixel[X] = 0;
		while (pixel[X] < game->double_buffer[NEXT]->width)
		{
			save_pixel_to_image(game->double_buffer[NEXT], pixel[X], pixel[Y],
				ground);
			pixel[X]++;
		}
		pixel[Y]++;
	}
}

void	render_bg(t_game *game)
{
	int	sky;
	int	ground;

	sky = t_color_to_int(&game->cub_data.ceiling_color, 255);
	ground = t_color_to_int(&game->cub_data.floor_color, 255);
	render_bg_color(game, sky, ground);
}
