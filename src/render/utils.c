/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 23:31:07 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/07 21:32:29 by marcnava         ###   ########.fr       */
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
	if (!isfinite(angle))
		return (0.0f);
	angle = fmodf(angle + FT_PI, TAU);
	if (angle < 0.0f)
		angle += TAU;
	return (angle - FT_PI);
}

float	clamp(float value, float min, float max)
{
	if (value < min)
		value = min;
	if (value > max)
		value = max;
	return (value);
}

void	safe_put_pixel(mlx_image_t *img, int x, int y, unsigned int color)
{
	if ((unsigned int)x < img->width && (unsigned int)y < img->height)
		save_pixel_to_image(img, x, y, color);
}
