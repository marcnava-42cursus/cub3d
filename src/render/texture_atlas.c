/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_atlas.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/28 11:23:30 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/30 23:54:59 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

void	paint_current_frame_to_image(mlx_image_t *img, t_atlas *atlas,
		int coord[2], unsigned int current_frame[2], float scale)
{
	uint8_t			*pixel_start;
	uint8_t			*row;
	unsigned int	pixel[2];
	unsigned int	stride;
	int				dst[2];
	int				src[2];

	if (!atlas || !atlas->xpm)
		return ;
	dst[X] = (int)(atlas->frame_width * scale);
	dst[Y] = (int)(atlas->frame_height * scale);
	stride = atlas->xpm->texture.width * atlas->xpm->texture.bytes_per_pixel;
	pixel_start = get_pixel_start(stride, current_frame, atlas);
	pixel[Y] = 0;
	while (pixel[Y] < dst[Y])
	{
		src[Y] = (int)(pixel[Y] / scale);
		row = pixel_start + src[Y] * stride;
		pixel[X] = 0;
		while (pixel[X] < dst[X])
		{
			src[X] = (int)(pixel[X] / scale);
			safe_put_pixel(img, coord[X] + pixel[X], coord[Y] + pixel[Y],
				get_corrected_color_from_pixel(row, src[X], atlas));
			pixel[X]++;
		}
		pixel[Y]++;
	}
}

void	paint_hori_flip_current_frame_to_image(mlx_image_t *img, t_atlas *atlas,
		int coord[2], unsigned int current_frame[2], float scale)
{
	uint8_t			*pixel_start;
	uint8_t			*row;
	unsigned int	pixel[2];
	unsigned int	horiflip;
	unsigned int	stride;
	int				dst[2];
	int				src[2];

	if (!atlas || !atlas->xpm)
		return ;
	dst[X] = (int)(atlas->frame_width * scale);
	dst[Y] = (int)(atlas->frame_height * scale);
	stride = atlas->xpm->texture.width * atlas->xpm->texture.bytes_per_pixel;
	pixel_start = get_pixel_start(stride, current_frame, atlas);
	pixel[Y] = 0;
	while (pixel[Y] < dst[Y])
	{
		src[Y] = (int)(pixel[Y] / scale);
		row = pixel_start + src[Y] * stride;
		pixel[X] = 0;
		while (horiflip >= 0 && pixel[X] < dst[X])
		{
			src[X] = (int)(pixel[X] / scale);
			horiflip = (atlas->frame_height - 1) - src[X];
			safe_put_pixel(img, coord[X] + pixel[X], coord[Y] + pixel[Y],
				get_corrected_color_from_pixel(row, horiflip, atlas));
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
