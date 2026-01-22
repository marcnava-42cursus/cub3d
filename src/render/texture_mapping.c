/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_mapping.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 01:50:36 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/22 11:22:52 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static int	get_tex_x(t_rayhit rayhit, t_textures *textures, xpm_t **texture)
{
	float	wall_x;

	if (rayhit.face == NORTH)
		*texture = textures->north;
	else if (rayhit.face == SOUTH)
		*texture = textures->south;
	else if (rayhit.face == EAST)
		*texture = textures->east;
	else if (rayhit.face == WEST)
		*texture = textures->west;
	else
		*texture = NULL;
	if (rayhit.side == 0)
		wall_x = rayhit.position.y / WORLDMAP_TILE_SIZE;
	else
		wall_x = rayhit.position.x / WORLDMAP_TILE_SIZE;
	wall_x = wall_x - floorf(wall_x);
	return ((int)(wall_x * (float)(*texture)->texture.width));
}

uint32_t	sample_texture_pixel(xpm_t *texture, int tex_x, float tex_pos)
{
	int			tex_y;
	int			pixel_index;
	uint8_t		*pixels;
	uint32_t	pixel_color;

	tex_y = (int)tex_pos;
	if (tex_y >= (int)texture->texture.height)
		tex_y = texture->texture.height - 1;
	if (tex_x >= 0 && tex_x < (int)texture->texture.width
		&& tex_y >= 0 && tex_y < (int)texture->texture.height)
	{
		pixel_index = (tex_y * texture->texture.width + tex_x)
			* texture->texture.bytes_per_pixel;
		if (pixel_index < (int)(texture->texture.width * texture->texture.height
			* texture->texture.bytes_per_pixel))
		{
			pixels = texture->texture.pixels;
			pixel_color = (pixels[pixel_index] << 24)
				| (pixels[pixel_index + 1] << 16)
				| (pixels[pixel_index + 2] << 8)
				| pixels[pixel_index + 3];
			return (pixel_color);
		}
	}
	return (MAGENTA);
}

// Textured vertical line painter - y[0] is start and y[1] is end
void	paint_vertical_line_texture(unsigned int x, int y[2], mlx_image_t *img,
		xpm_t *texture, int tex_x, float tex_pos, float tex_step)
{
	int			current_y;
	float		current_tex_pos;
	uint32_t	pixel_color;

	current_y = y[0];
	current_tex_pos = tex_pos;
	while (current_y <= y[1])
	{
		pixel_color = sample_texture_pixel(texture, tex_x, current_tex_pos);
		save_pixel_to_image(img, x, (unsigned int)current_y, pixel_color);
		current_y++;
		current_tex_pos += tex_step;
	}
}

void	paint_horizontal_line_texture(unsigned int y, unsigned int x,
		mlx_image_t *img, xpm_t *texture, int tex_y, float tex_x)
{
	uint32_t	pixel_color;

	pixel_color = sample_texture_pixel(texture, tex_x, tex_y);
	save_pixel_to_image(img, x, y, pixel_color);
}

void	render_texture_line(t_rayhit rayhit, unsigned int x, int y[2],
		mlx_image_t *img, t_textures *textures)
{
	xpm_t		*texture;
	int			tex_x;
	int			original_line_height;
	float		step;
	float		tex_offset;

	if (x >= img->width)
		return ;
	if (y[1] >= (int)img->height)
		y[1] = (int)img->height - 1;
	if (y[0] >= y[1])
		return ;
	tex_x = get_tex_x(rayhit, textures, &texture);
	if ((rayhit.side == 0 && rayhit.face == NORTH)
		|| (rayhit.side == 1 && rayhit.face == WEST))
		tex_x = texture->texture.width - tex_x - 1;
	original_line_height = rayhit.wall_bounds[1] - rayhit.wall_bounds[0] + 1;
	step = (float)texture->texture.height / (float)original_line_height;
	tex_offset = (y[0] - rayhit.wall_bounds[0]) * step;
	paint_vertical_line_texture(x, y, img, texture, tex_x, tex_offset, step);
}
