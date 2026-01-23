/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 23:31:07 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/23 10:31:25 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

void	save_pixel_to_image(mlx_image_t *image, unsigned int x, unsigned int y,
			uint32_t color)
{
	uint8_t			*pixelstart;
	unsigned int	bpp;

	if ((color & 0xFFu) == 0)
		return ;
	bpp = 4;
	pixelstart = &image->pixels[(y * image->width + x) * bpp];
	pixelstart[0] = (color >> 24) & 0xFF;
	pixelstart[1] = (color >> 16) & 0xFF;
	pixelstart[2] = (color >> 8) & 0xFF;
	pixelstart[3] = color & 0xFF;
}

int	t_color_to_int(t_color *color, int alpha)
{
	return (color->r << 24 | color->g << 16 | color->b << 8 | alpha);
}

float	normalize_angle(float angle)
{
	while (angle > FT_PI)
		angle -= TAU;
	while (angle <= -FT_PI)
		angle += TAU;
	return (angle);
}

float	clamp(float value, float min, float max)
{
	if (value < min)
		value = min;
	if (value > max)
		value = max;
	return (value);
}

// Simple solid color vertical line painter - y[0] is start and y[1] is end
void	paint_vertical_line_color(unsigned int x, int y[2], mlx_image_t *img,
		uint32_t color)
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

void	safe_put_pixel(mlx_image_t *img, int x, int y, unsigned int color)
{
	if ((unsigned int)x < img->width && (unsigned int)y < img->height)
		save_pixel_to_image(img, x, y, color);
}
