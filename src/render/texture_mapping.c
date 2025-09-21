/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_mapping.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 01:50:36 by ivmirand          #+#    #+#             */
/*   Updated: 2025/09/21 02:14:23 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

void render_texture_line(t_rayhit rayhit, int x, int y[2],
	mlx_image_t *img, t_textures *textures)
{
	int		current_y;
	int		color;
	char	*tex_to_paint;
	char	pixel_to_paint;

	if (rayhit.face == NORTH)
	{
		tex_to_paint = textures->north;
		color = LIGHT_GREY;
	}
	if (rayhit.face == SOUTH)
	{
		tex_to_paint = textures->south;
		color = BLACK;
	}
	if (rayhit.face == EAST)
	{
		tex_to_paint = textures->east;
		color = DARK_GREY;
	}
	if (rayhit.face == WEST)
	{
		tex_to_paint = textures->west;
		color = MEDIUM_GREY;
	}	
	if (x < 0 || x >= (int)img->width)
		return ;
	if (y[0] < 0)
		y[0] = 0;
	if (y[1] >= (int)img->height)
		y[1] = (int)img->height - 1;
	if (y[0] >= y[1])
		return ;
	current_y = y[0];
	if (!tex_to_paint || tex_to_paint[0] == '\0')
	{
		while (current_y <= y[1])
		{
			mlx_put_pixel(img, x, current_y, color);
			current_y++;
		}
	}
	else
	{
		while (current_y <= y[1])
		{
			pixel_to_paint = tex_to_paint[x + TEXTURE_WIDTH * current_y];
			mlx_put_pixel(img, x, current_y, pixel_to_paint);
			current_y++;
		}
	}
}
