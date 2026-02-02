/*                                                        :::      ::::::::   */
/*   texture_mapping_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/31 16:38:54 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "animation.h"

static xpm_t	*get_custom_texture(t_custom_texture *custom, char cell,
		t_orientation face)
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

//void	render_texture_line_bonus(t_rayhit rayhit, unsigned int x, int y[2],
//		int original_y[2], mlx_image_t *img, t_textures *textures,
//		const t_map *map)
//{
//	uint32_t	color;
//	xpm_t		*texture;
//	int			tex_x;
//	int			original_line_height;
//	float		wall_x;
//	float		step;
//	float		tex_offset;
//	char		cell;
//	float		fog;
//
//	texture = NULL;
//	color = WHITE;
//	if (rayhit.hit && rayhit.cell[1] >= 0 && rayhit.cell[1] < map->height
//		&& rayhit.cell[0] >= 0
//		&& rayhit.cell[0] < (int)ft_strlen(map->grid[rayhit.cell[1]]))
//	{
//		cell = map->grid[rayhit.cell[1]][rayhit.cell[0]];
//		if (textures->custom)
//			texture = get_custom_texture(textures->custom, cell, rayhit.face);
//	}
//	if (!texture)
//	{
//		if (rayhit.face == NORTH)
//		{
//			texture = textures->north;
//			color = LIGHT_GREY;
//		}
//		else if (rayhit.face == SOUTH)
//		{
//			texture = textures->south;
//			color = BLACK;
//		}
//		else if (rayhit.face == EAST)
//		{
//			texture = textures->east;
//			color = DARK_GREY;
//		}
//		else if (rayhit.face == WEST)
//		{
//			texture = textures->west;
//			color = MEDIUM_GREY;
//		}
//	}
//	if (x >= img->width)
//		return ;
//	if (y[1] >= (int)img->height)
//		y[1] = (int)img->height - 1;
//	if (y[0] >= y[1])
//		return ;
//	if (texture)
//	{
//		if (rayhit.side == 0)
//			wall_x = rayhit.position.y / WORLDMAP_TILE_SIZE;
//		else
//			wall_x = rayhit.position.x / WORLDMAP_TILE_SIZE;
//		wall_x = wall_x - floorf(wall_x);
//		tex_x = (int)(wall_x * (float)texture->texture.width);
//		if ((rayhit.side == 0 && rayhit.face == NORTH)
//			|| (rayhit.side == 1 && rayhit.face == WEST))
//			tex_x = texture->texture.width - tex_x - 1;
//		original_line_height = original_y[1] - original_y[0] + 1;
//		step = (float)texture->texture.height / (float)original_line_height;
//		tex_offset = (y[0] - original_y[0]) * step;
//		fog = fog_factor(rayhit.distance);
//		fog = fog * fog;
//		paint_vertical_line_texture_bonus(x, y, img, texture, textures->fog,
//			tex_x, tex_offset, step, fog);
//	}
//	else
//		paint_vertical_line_color(x, y, img, color);
//}

static int	get_tex_x(t_game *game, t_rayhit *rayhit, xpm_t **texture)
{
	float	wall_x;
	char	cell_char;
	int		frame_width;

	cell_char = game->cub_data.map.grid[rayhit->cell[Y]][rayhit->cell[X]];
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
	if (cell_char == '2')
	{
		*texture = game->cub_data.block.atlas.xpm;
		frame_width = game->cub_data.block.atlas.frame_width;
	}
	else if (ft_strchr("!\"·$%&/()=?", cell_char))
	{
		*texture = game->cub_data.effects.door_atlas.xpm;
		frame_width = game->cub_data.effects.door_atlas.frame_width;
	}
	else
		frame_width = (*texture)->texture.width;
	if (rayhit->side == 0)
		wall_x = rayhit->position.y / WORLDMAP_TILE_SIZE;
	else
		wall_x = rayhit->position.x / WORLDMAP_TILE_SIZE;
	wall_x = wall_x - floorf(wall_x);
	return ((int)(wall_x * (float)frame_width));
}

uint32_t	sample_atlas_frame_pixel(t_atlas *atlas,
		unsigned int current_frame[2], int tex_x, float tex_pos)
{
	int			tex_y;
	int			pixel_index;
	uint8_t		*pixels;
	int			src[2];

	tex_y = (int)tex_pos;
	tex_x = (int)clamp((float)tex_x, 0.0f, (float)(atlas->frame_width - 1));
	tex_y = (int)clamp((float)tex_y, 0.0f, (float)(atlas->frame_height - 1));
	src[X] = (current_frame[X] * 128.0f) + tex_x;
	src[Y] = (current_frame[Y] * 128.0f) + tex_y;
	if (src[X] < 0 || src[X] >= (int)atlas->xpm->texture.width
		|| src[Y] < 0 || src[Y] >= (int)atlas->xpm->texture.height)
		return (MAGENTA);
	pixels = atlas->xpm->texture.pixels;
	pixel_index = (src[Y] * atlas->xpm->texture.width + src[X])
		* atlas->xpm->texture.bytes_per_pixel;
	return ((pixels[pixel_index] << 24)
		| (pixels[pixel_index + 1] << 16)
		| (pixels[pixel_index + 2] << 8)
		| pixels[pixel_index + 3]);
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
void	paint_vertical_line_texture_bonus(unsigned int x, int y[2],
		mlx_image_t *img, xpm_t *texture, xpm_t *fog_texture,
		int tex_x, float tex_pos, float tex_step, float fog, t_atlas *atlas,
		unsigned int current_frame[2])
{
	int			current_y;

	current_y = y[0];
	while (current_y <= y[1])
	{
		paint_horizontal_line_texture_bonus(current_y, x, img, texture,
			fog_texture, tex_pos, tex_x, fog, atlas, current_frame);
		current_y++;
		tex_pos += tex_step;
	}
}

void	paint_horizontal_line_texture_bonus(unsigned int y, unsigned int x,
		mlx_image_t *img, xpm_t *texture, xpm_t *fog_texture, int tex_y,
		float tex_x, float fog, t_atlas *atlas, unsigned int current_frame[2])
{
	uint32_t	pixel_color;
	int			fog_pixel[2];
	uint32_t	fog_sample;
	float		fog_mod;
	float		noise;
	float		strength;

	if (atlas)
		pixel_color = sample_atlas_frame_pixel(atlas, current_frame, tex_x,
				tex_y);
	else
		pixel_color = sample_texture_pixel(texture, tex_x, tex_y);
	if (fog_texture)
	{
		fog_pixel[X] = (int)(x & (fog_texture->texture.width - 1));
		fog_pixel[Y] = (int)(y & (fog_texture->texture.height - 1));
		fog_sample = sample_texture_pixel(fog_texture, fog_pixel[X],
				fog_pixel[Y]);
		noise = ((fog_sample >> 24) & 0xFF) * (1.0f / 255.0f);
		noise -= 0.5f;
		strength = 0.05f + 0.15f * fog;
		fog_mod = fog + noise * (strength * fog);
		fog_mod = clamp(fog_mod, 0.0f, 0.95f);
		fog_sample = rgba_color_lerp(pixel_color, FOG_COLOR, fog_mod);
	}
	else
		fog_sample = rgba_color_lerp(pixel_color, FOG_COLOR, fog);
	save_pixel_to_image(img, x, y, fog_sample);
}

void	render_texture_line_bonus(t_rayhit *rayhit, unsigned int x, int y[2],
		t_game *game)
{
	xpm_t	*texture;
	int		tex_x;
	int		original_line_height;
	float	step;
	float	tex_offset;
	float	fog;
	int		y_unclipped[2];
	int		source_height;
	int		source_width;
	unsigned int		current_frame[2] = {0, 0};

	y_unclipped[0] = rayhit->wall_bounds[0];
	y_unclipped[1] = rayhit->wall_bounds[1];
	original_line_height = y_unclipped[1] - y_unclipped[0] + 1;
	y[0] = y_unclipped[0];
	y[1] = y_unclipped[1];
	if (x >= game->double_buffer[NEXT]->width)
		return ;
	if (y[0] < 0)
		y[0] = 0;
	if (y[1] >= (int)game->double_buffer[NEXT]->height)
		y[1] = (int)game->double_buffer[NEXT]->height - 1;
	if (y[0] >= y[1])
		return ;
	tex_x = get_tex_x(game, rayhit, &texture);
	if (texture == game->cub_data.block.atlas.xpm)
	{
		source_height = game->cub_data.block.atlas.frame_height;
		source_width = game->cub_data.block.atlas.frame_width;
	}
	else if (texture == game->cub_data.effects.door_atlas.xpm)
	{
		source_height = game->cub_data.effects.door_atlas.frame_height;
		source_width = game->cub_data.effects.door_atlas.frame_width;
	}
	else
	{
		source_height = texture->texture.height;
		source_width = texture->texture.width;
	}
	if ((rayhit->side == 0 && rayhit->face == NORTH)
		|| (rayhit->side == 1 && rayhit->face == WEST))
		tex_x = source_width - tex_x - 1;
	step = (float)source_height / (float)original_line_height;
	tex_offset = (y[0] - y_unclipped[0]) * step;
	fog = fog_factor(rayhit->distance);
	if (texture == game->cub_data.block.atlas.xpm)
		paint_vertical_line_texture_bonus(x, y, game->double_buffer[NEXT],
			texture, game->cub_data.textures.fog, tex_x, tex_offset, step, fog,
			&game->cub_data.block.atlas,
			game->cub_data.block.anims[ANIM_BREATHE].current_frame);
	else if (texture == game->cub_data.effects.door_atlas.xpm)
		paint_vertical_line_texture_bonus(x, y, game->double_buffer[NEXT],
			texture, game->cub_data.textures.fog, tex_x, tex_offset, step, fog,
			&game->cub_data.effects.door_atlas,
			//game->cub_data.effects.door_anims[ANIM_BREATHE].current_frame);
			current_frame);
	else
		paint_vertical_line_texture_bonus(x, y, game->double_buffer[NEXT],
			texture, game->cub_data.textures.fog, tex_x, tex_offset, step, fog,
			NULL, NULL);
}
