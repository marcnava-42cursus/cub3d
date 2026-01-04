/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_atlas.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/28 11:23:30 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/04 16:59:11 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

void	paint_current_frame_to_image(mlx_image_t *img, t_atlas *atlas,
		int coord[2], int current_frame[2])
{
	uint8_t			*pxl_start;
	uint8_t			*row;
	unsigned int	pixel[2];
	unsigned int	stride;
	uint32_t		color;

	if (!atlas || !atlas->xpm)
		return ;
	stride = atlas->xpm->texture.width * atlas->xpm->texture.bytes_per_pixel;
	pixel[Y] = current_frame[Y] * atlas->frame_height;
	pixel[X] = current_frame[X] * atlas->frame_width;
	pxl_start = &atlas->xpm->texture.pixels[pixel[Y] * stride + pixel[X]
		* atlas->xpm->texture.bytes_per_pixel];
	pixel[Y] = 0;
	while (pixel[Y] < atlas->frame_height)
	{
		row = pxl_start + pixel[Y] * stride;
		pixel[X] = 0;
		while (pixel[X] < atlas->frame_width)
		{
			uint8_t	*p = row + pixel[X] * atlas->xpm->texture.bytes_per_pixel;
			color = ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16)
				| ((uint32_t)p[2] << 8) | (uint32_t)p[3];
			save_pixel_to_image(img, coord[X] + pixel[X], coord[Y] + pixel[Y],
				color);
			pixel[X]++;
		}
		pixel[Y]++;
	}
}

void	paint_hori_flip_current_frame_to_image(mlx_image_t *img, t_atlas *atlas,
		int coord[2], int current_frame[2])
{
	uint8_t			*pxl_start;
	uint8_t			*row;
	unsigned int	pixel[2];
	unsigned int	horiflip;
	unsigned int	stride;
	uint32_t		color;

	if (!atlas || !atlas->xpm)
		return ;
	stride = atlas->xpm->texture.width * atlas->xpm->texture.bytes_per_pixel;
	pixel[Y] = current_frame[Y] * atlas->frame_height;
	pixel[X] = current_frame[X] * atlas->frame_width;
	pxl_start = &atlas->xpm->texture.pixels[pixel[Y] * stride + pixel[X]
		* atlas->xpm->texture.bytes_per_pixel];
	pixel[Y] = 0;
	while (pixel[Y] < atlas->frame_height)
	{
		row = pxl_start + pixel[Y] * stride;
		horiflip = atlas->frame_width - 1;
		pixel[X] = 0;
		while (horiflip >= 0 && pixel[X] < atlas->frame_width)
		{
			uint8_t	*p = row + horiflip * atlas->xpm->texture.bytes_per_pixel;
			color = ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16)
				| ((uint32_t)p[2] << 8) | (uint32_t)p[3];
			save_pixel_to_image(img, coord[X] + pixel[X], coord[Y] + pixel[Y],
				color);
			horiflip--;
			pixel[X]++;
		}
		pixel[Y]++;
	}
}

void	atlas_init(t_atlas *atlas, char *xpm_path, unsigned int frame_width,
		unsigned int frame_height)
{
	atlas->xpm = mlx_load_xpm42(xpm_path);
	atlas->frame_width = frame_width;
	atlas->frame_height = frame_height;
	atlas->max_frame[X] = atlas->xpm->texture.width / atlas->frame_width;
	atlas->max_frame[Y] = atlas->xpm->texture.height / atlas->frame_height;
	atlas->total_frames = atlas->max_frame[X] * atlas->max_frame[Y];
}

void	atlas_free(t_atlas *atlas)
{
	if (atlas->xpm != NULL)
	{
		mlx_delete_xpm42(atlas->xpm);
		atlas->xpm = NULL;
	}
}
