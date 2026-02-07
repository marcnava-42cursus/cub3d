/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_mapping.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 01:50:36 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/07 22:33:34 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static void	get_tex_x(t_game *game, t_rayhit *rayhit, xpm_t **texture,
		float x_offset_step[3])
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
	x_offset_step[2] = (float)((*texture)->texture.height / x_offset_step[1]);
	x_offset_step[0] = (float)(wall_x * (float)(*texture)->texture.width);
	if ((rayhit->side == 0 && rayhit->face == NORTH)
		|| (rayhit->side == 1 && rayhit->face == WEST))
		x_offset_step[0] = (float)((*texture)->texture.width - x_offset_step[0]
				- 1);
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
void	paint_vertical_line_texture(int x_y_packed[3], mlx_image_t *img,
		xpm_t *texture, float x_offset_step[3])
{
	unsigned int	pixel[2];
	int				tex_coord[2];

	pixel[X] = x_y_packed[0];
	pixel[Y] = (unsigned int)x_y_packed[1];
	tex_coord[X] = (int)x_offset_step[0];
	while (pixel[Y] <= (unsigned int)x_y_packed[2])
	{
		tex_coord[Y] = (int)x_offset_step[1];
		paint_texture_pixel(pixel, img, tex_coord, texture);
		pixel[Y]++;
		x_offset_step[1] += x_offset_step[2];
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
	int		x_y_packed[3];
	xpm_t	*texture;
	float	x_offset_step[3];
	float	original_line_height;

	if (x >= game->double_buffer[NEXT]->width)
		return ;
	if (y[1] >= (int)game->double_buffer[NEXT]->height)
		y[1] = (int)game->double_buffer[NEXT]->height - 1;
	if (y[0] >= y[1])
		return ;
	original_line_height = (float)(rayhit->wall_bounds[1]
			- rayhit->wall_bounds[0] + 1);
	x_offset_step[1] = original_line_height;
	get_tex_x(game, rayhit, &texture, x_offset_step);
	x_offset_step[1] = (float)((y[0] - rayhit->wall_bounds[0])
			* x_offset_step[2]);
	pack_x_ys((int)x, y, x_y_packed);
	paint_vertical_line_texture(x_y_packed, game->double_buffer[NEXT], texture,
		x_offset_step);
}
