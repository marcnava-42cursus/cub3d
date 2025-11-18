/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 23:31:07 by ivmirand          #+#    #+#             */
/*   Updated: 2025/09/29 00:29:29 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

void	save_pixel_to_image(mlx_image_t *image, unsigned int x, unsigned int y,
			unsigned int color)
{
	unsigned char *pixelstart;

	pixelstart = &image->pixels[(y * image->width + x) * sizeof(unsigned int)];
	*(pixelstart++) = (unsigned short)(color >> 24);
	*(pixelstart++) = (unsigned short)(color >> 16);
	*(pixelstart++) = (unsigned short)(color >> 8);
	*(pixelstart++) = (unsigned short)(color & 255);
}

int	t_color_to_int(t_color *color, int alpha)
{
	return (color->r << 24 | color->g << 16 | color->b << 8 | alpha);
}

int	clamp(int value, int min, int max)
{
	if (value < min)
		return (min);
	else if (value > max)
		return (max);
	else
		return (value);
}

float	normalize_angle(float angle)
{
	while (angle > FT_PI)
		angle -= TAU;
	while (angle <= -FT_PI)
		angle += TAU;
	return (angle);
}

// Simple solid color vertical line painter - y[0] is start and y[1] is end
void paint_vertical_line_color(unsigned int x, int y[2], mlx_image_t *img, uint32_t color)
{
	int	current_y;
	
	if (x >= img->width)
		return ;
	if (y[1] >= (int)img->height)
		y[1] = (int)img->height - 1;
	if (y[0] < 0)
		y[0] = 0;
	if (y[0] >= y[1])
		return ;
	current_y = y[0];
	while (current_y <= y[1])
	{
		save_pixel_to_image(img, x, (unsigned int)current_y, color);
		current_y++;
	}
}
