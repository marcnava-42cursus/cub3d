/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_atlas.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/28 11:23:30 by ivmirand          #+#    #+#             */
/*   Updated: 2025/12/31 21:24:32 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "animation.h"

void	paint_current_frame_to_image(mlx_image_t *img, t_atlas *atlas,
		int coord_x, int coord_y)
{
	uint8_t			*pxl_start;
	uint8_t			*row;
	unsigned int	pixel[2];
	unsigned int	stride;
	uint32_t		color;

	stride = atlas->xpm->texture.width * atlas->xpm->texture.bytes_per_pixel;
	pixel[Y] = atlas->current_frame[Y] * atlas->frame_height;
	pixel[X] = atlas->current_frame[X] * atlas->frame_width;
	pxl_start = &atlas->xpm->texture.pixels[pixel[Y] * stride + pixel[X]
		* atlas->xpm->texture.bytes_per_pixel];
	pixel[Y] = 0;
	while (pixel[Y] < atlas->frame_height)
	{
		row = pxl_start + pixel[Y] * stride;
		pixel[X] = 0;
		while (pixel[X] < atlas->frame_width)
		{
			color = (row[pixel[X] * atlas->xpm->texture.bytes_per_pixel] << 24)
				| (row[pixel[X] * atlas->xpm->texture.bytes_per_pixel + 1]
					<< 16)
				| (row[pixel[X] * atlas->xpm->texture.bytes_per_pixel + 2] << 8)
				| row[pixel[X] * atlas->xpm->texture.bytes_per_pixel + 3];
			save_pixel_to_image(img, coord_x, coord_y, color);	
			pixel[X]++;
		}
		pixel[Y]++;
	}
}

void	get_next_frame(t_atlas *atlas, int increment)
{
	unsigned int	x;
	unsigned int	y;

	x = atlas->current_frame[X] += increment;
	y = atlas->current_frame[Y];
	y += x / atlas->max_frame[X];
	x %= atlas->max_frame[X];
	y %= atlas->max_frame[Y];
	atlas->current_frame[X] = x;
	atlas->current_frame[Y] = y;
}

void	atlas_init(t_atlas *atlas, char *xpm_path, unsigned int frame_width,
		unsigned int frame_height)
{
	atlas->xpm_path = xpm_path;
	atlas->xpm = mlx_load_xpm42(xpm_path);
	atlas->frame_width = frame_width;
	atlas->frame_height = frame_height;
	atlas->max_frame[X] = atlas->xpm->texture.width / atlas->frame_width;
	atlas->max_frame[Y] = atlas->xpm->texture.height / atlas->frame_height;
	atlas->current_frame[X] = 0;
	atlas->current_frame[Y] = 0;
	atlas->total_frames = atlas->max_frame[X] * atlas->max_frame[Y];
	atlas->current_frame[X] = 0;
	atlas->current_frame[Y] = 0;
}
