/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_mapping.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 01:50:36 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/07 13:55:19 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static int	get_tex_x(t_game *game, t_rayhit *rayhit, xpm_t **texture,
		float original_line_height, int *step)
{
	float	wall_x;

	if (rayhit->face == NORTH)
		*texture = game->cub_data.textures.north;
	else if (rayhit->face == SOUTH)
		*texture = game->cub_data.textures.south;
	else if (rayhit->face == EAST)
		*texture = game->cub_data.textures.east;
	else if (rayhit->face == WEST)
		*texture = game->cub_data.textures.west;
	else
		*texture = NULL;
	if (rayhit->side == 0)
		wall_x = rayhit->position.y / WORLDMAP_TILE_SIZE;
	else
		wall_x = rayhit->position.x / WORLDMAP_TILE_SIZE;
	wall_x = wall_x - floorf(wall_x);
	step = (float)(*texture)->texture.height / (float)*original_line_height;
	return ((int)(wall_x * (float)(*texture)->texture.width));
}

uint32_t	sample_texture_pixel(xpm_t *texture, int tex_x, float tex_pos)
{
	int			tex_y;
	int			pixel_index;
	uint8_t		*pixels;
	uint32_t	pixel_color;

	tex_y = (int)tex_pos;
	tex_y = (int)clamp((float)tex_y, 0.0f,
			(float)(texture->texture.height - 1));
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
	unsigned int	pixel[2];
	int				tex_coord[2];

	pixel[X] = x;
	pixel[Y] = (unsigned int)y[0];
	tex_coord[X] = tex_x;
	while (pixel[Y] <= (unsigned int)y[1])
	{
		tex_coord[Y] = (int)tex_pos;
		paint_texture_pixel(pixel, img, tex_coord, texture);
		//paint_horizontal_line_texture(current_y, x, img, texture, tex_pos,
		//	tex_x);
		pixel[Y]++;
		tex_pos += tex_step;
	}
}

void	paint_texture_pixel(unsigned int pixel[2], mlx_image_t *img,
		int tex_coord[2], xpm_t *texture)
{
	uint32_t	pixel_color;

	pixel_color = sample_texture_pixel(texture, tex_coord[X], tex_coord[Y]);
	save_pixel_to_image(img, pixel[X], pixel[Y], pixel_color);
}

void	render_texture_line(t_rayhit *rayhit, unsigned int x, int y[2],
		t_game *game)
{
	xpm_t	*texture;
	int		x_offset_step[3];
	float	original_line_height;

	if (x >= game->double_buffer[NEXT]->width)
		return ;
	if (y[1] >= (int)game->double_buffer[NEXT]->height)
		y[1] = (int)game->double_buffer[NEXT]->height - 1;
	if (y[0] >= y[1])
		return ;
	x_offset_step[2] = (float)(rayhit->wall_bounds[1] - rayhit->wall_bounds[0] + 1);
	x_offset_step[0] = get_tex_x(game, rayhit, &texture, original_line_height);
	if ((rayhit->side == 0 && rayhit->face == NORTH)
		|| (rayhit->side == 1 && rayhit->face == WEST))
		x_offset_step[0] = texture->texture.width - tex_x - 1;
	tex_offset = (y[0] - rayhit->wall_bounds[0]) * step;
	paint_vertical_line_texture(x, y, game->double_buffer[NEXT],
		texture, tex_x, tex_offset, step);
}
