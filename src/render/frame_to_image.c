/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frame_to_image.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/28 11:23:30 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/06 19:53:50 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static unsigned int	get_hori_flip(t_atlas *atlas, int source_x)
{
	return ((atlas->frame_height - 1) - source_x);
}

void	frame_to_image(mlx_image_t *img, t_atlas *atlas,
		int coord[2], unsigned int frame[2])
{
	uint8_t			*pixel_start;
	uint8_t			*row;
	unsigned int	stride;
	int				pixel[2];

	if (!atlas || !atlas->xpm)
		return ;
	stride = atlas->xpm->texture.width * atlas->xpm->texture.bytes_per_pixel;
	pixel_start = get_pixel_start(stride, frame, atlas);
	pixel[Y] = 0;
	while (pixel[Y] < (int)(atlas->frame_height))
	{
		row = pixel_start + pixel[Y] * stride;
		pixel[X] = 0;
		while (pixel[X] < (int)(atlas->frame_height))
		{
			safe_put_pixel(img, coord[X] + pixel[X], coord[Y] + pixel[Y],
				get_corrected_color_from_pixel(row, pixel[X], atlas));
			pixel[X]++;
		}
		pixel[Y]++;
	}
}

void	hori_flip_frame_to_image(mlx_image_t *img, t_atlas *atlas,
		int coord[2], unsigned int frame[2])
{
	uint8_t			*pixel_start;
	uint8_t			*row;
	unsigned int	pixel[2];
	unsigned int	stride;

	if (!atlas || !atlas->xpm)
		return ;
	stride = atlas->xpm->texture.width * atlas->xpm->texture.bytes_per_pixel;
	pixel_start = get_pixel_start(stride, frame, atlas);
	pixel[Y] = 0;
	while (pixel[Y] < (int)atlas->frame_height)
	{
		row = pixel_start + pixel[Y] * stride;
		pixel[X] = 0;
		while (get_hori_flip(atlas, pixel[X]) >= 0
			&& pixel[X] < (int)atlas->frame_width)
		{
			safe_put_pixel(img, coord[X] + pixel[X], coord[Y] + pixel[Y],
				get_corrected_color_from_pixel(row, get_hori_flip(atlas,
						pixel[X]), atlas));
			pixel[X]++;
		}
		pixel[Y]++;
	}
}
