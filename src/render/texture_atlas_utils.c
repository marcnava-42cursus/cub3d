/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_atlas_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 17:27:44 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/04 18:08:18 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

uint8_t	*get_pixel_start(unsigned int stride, int current_frame[2],
		t_atlas *atlas)
{
	uint8_t			*pixel_start;
	unsigned int	pixel[2];

	pixel[Y] = current_frame[Y] * atlas->frame_height;
	pixel[X] = current_frame[X] * atlas->frame_width;
	pixel_start = &atlas->xpm->texture.pixels[pixel[Y] * stride + pixel[X]
		* atlas->xpm->texture.bytes_per_pixel];
	return (pixel_start);
}

uint32_t	get_corrected_color_from_pixel(uint8_t *row, unsigned int x,
		t_atlas *atlas)
{
	uint8_t		*p;
	uint32_t	color;

	p = row + x * atlas->xpm->texture.bytes_per_pixel;
	color = ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16)
		| ((uint32_t)p[2] << 8) | (uint32_t)p[3];
	return (color);
}
