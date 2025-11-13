/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_mapping_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/11/13 20:03:53 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static uint32_t	sample_texture_pixel(xpm_t *texture, int tex_x, float tex_pos)
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
		if (pixel_index < (int)(texture->texture.width
			* texture->texture.height
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

void paint_vertical_line_texture(unsigned int x, int y[2], mlx_image_t *img,
		xpm_t *texture, int tex_x, float tex_pos, float tex_step)
{
	int	current_y;
	float	current_tex_pos;

	if (x >= img->width)
		return ;
	if (y[1] >= (int)img->height)
		y[1] = (int)img->height - 1;
	if (y[0] < 0)
		y[0] = 0;
	if (y[0] >= y[1])
		return ;
	current_y = y[0];
	current_tex_pos = tex_pos;
	while (current_y <= y[1])
	{
		uint32_t pixel_color = sample_texture_pixel(texture, tex_x, current_tex_pos);
		save_pixel_to_image(img, x, (unsigned int)current_y, pixel_color);
		current_y++;
		current_tex_pos += tex_step;
	}
}

static xpm_t	*get_custom_texture(t_custom_texture *custom, char cell, t_orientation face)
{
	t_custom_texture	*current;
	char				id[3];

	id[0] = cell;
	if (face == NORTH)
		id[1] = 'N';
	else if (face == SOUTH)
		id[1] = 'S';
	else if (face == EAST)
		id[1] = 'E';
	else if (face == WEST)
		id[1] = 'W';
	else
		return (NULL);
	id[2] = '\0';
	current = custom;
	while (current)
	{
		if (ft_strcmp(current->id, id) == 0)
			return (current->texture);
		current = current->next;
	}
	return (NULL);
}

void	render_texture_line_bonus(t_rayhit rayhit, unsigned int x, int y[2],
		int original_y[2], mlx_image_t *img, t_textures *textures, const t_map *map)
{
	uint32_t	color;
	xpm_t		*texture;
	int			tex_x;
	int			original_line_height;
	float		wall_x;
	float		step;
	float		tex_offset;
	char		cell;

	texture = NULL;
	color = WHITE;

	if (rayhit.hit && rayhit.cell_y >= 0 && rayhit.cell_y < map->height
		&& rayhit.cell_x >= 0 && rayhit.cell_x < (int)ft_strlen(map->grid[rayhit.cell_y]))
	{
		cell = map->grid[rayhit.cell_y][rayhit.cell_x];

		if (cell != '1' && textures->custom)
			texture = get_custom_texture(textures->custom, cell, rayhit.face);
	}

	if (!texture)
	{
		if (rayhit.face == NORTH)
		{
			texture = textures->north;
			color = LIGHT_GREY;
		}
		else if (rayhit.face == SOUTH)
		{
			texture = textures->south;
			color = BLACK;
		}
		else if (rayhit.face == EAST)
		{
			texture = textures->east;
			color = DARK_GREY;
		}
		else if (rayhit.face == WEST)
		{
			texture = textures->west;
			color = MEDIUM_GREY;
		}
	}

	if (x >= img->width)
		return ;
	if (y[1] >= (int)img->height)
		y[1] = (int)img->height - 1;
	if (y[0] >= y[1])
		return ;

	if (texture)
	{
		if (rayhit.side == 0)
			wall_x = rayhit.position.y / WORLDMAP_TILE_SIZE;
		else
			wall_x = rayhit.position.x / WORLDMAP_TILE_SIZE;
		wall_x = wall_x - floorf(wall_x);

		tex_x = (int)(wall_x * (float)texture->texture.width);
		if ((rayhit.side == 0 && rayhit.face == NORTH)
			|| (rayhit.side == 1 && rayhit.face == WEST))
			tex_x = texture->texture.width - tex_x - 1;

		original_line_height = original_y[1] - original_y[0] + 1;
		step = (float)texture->texture.height / (float)original_line_height;
		tex_offset = (y[0] - original_y[0]) * step;

		paint_vertical_line_texture(x, y, img, texture, tex_x, tex_offset, step);
	}
	else
		paint_vertical_line_color(x, y, img, color);
}
