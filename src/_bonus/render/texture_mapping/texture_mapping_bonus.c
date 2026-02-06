/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_mapping_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/06 18:03:49 by ivmirand         ###   ########.fr       */
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

static int	get_tex_x(t_game *game, t_rayhit *rayhit, xpm_t **texture,
		float *original_line_height)
{
	int		tex_x;
	float	wall_x;
	char	cell_char;
	int		source_wh[2];

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
		*texture = game->cub_data.block.atlas.xpm;
	else if (ft_strchr("!\"·$%&/()=?", cell_char))
		*texture = game->cub_data.effects.door_atlas.xpm;
	if (*texture == game->cub_data.block.atlas.xpm)
	{
		source_wh[0] = game->cub_data.block.atlas.frame_width;
		source_wh[1] = game->cub_data.block.atlas.frame_height;
	}
	else if (*texture == game->cub_data.effects.door_atlas.xpm)
	{
		source_wh[0] = game->cub_data.effects.door_atlas.frame_width;
		source_wh[1] = game->cub_data.effects.door_atlas.frame_height;
	}
	else
	{
		source_wh[0] = (*texture)->texture.width;
		source_wh[1] = (*texture)->texture.height;
	}
	if (rayhit->side == 0)
		wall_x = rayhit->position.y / WORLDMAP_TILE_SIZE;
	else
		wall_x = rayhit->position.x / WORLDMAP_TILE_SIZE;
	wall_x = wall_x - floorf(wall_x);
	tex_x = (int)(wall_x * (float)source_wh[0]);
	if ((rayhit->side == 0 && rayhit->face == NORTH)
		|| (rayhit->side == 1 && rayhit->face == WEST))
		tex_x = source_wh[0] - tex_x - 1;
	*original_line_height = (float)source_wh[1] / *original_line_height;
	return (tex_x);
}

// Textured vertical line painter - y[0] is start and y[1] is end
void	paint_vertical_line_texture_bonus(unsigned int x, int y[2],
		t_game *game, xpm_t *texture, int tex_x, float tex_pos, float tex_step,
		float fog, t_anim *anim)
{
	int	pixel[2];
	int				tex_coord[2];

	pixel[X] = (int)x;
	pixel[Y] = y[0];
	tex_coord[X] = tex_x;
	tex_coord[Y] = (int)tex_pos;
	while (pixel[Y] <= y[1])
	{
		paint_texture_pixel_bonus(pixel, game, texture, tex_coord, fog, anim);
		pixel[Y]++;
		tex_coord[Y] += (int)tex_step;
	}
}

void	render_texture_line_bonus(t_rayhit *rayhit, unsigned int x, int y[2],
		t_game *game)
{
	xpm_t				*texture;
	int					tex_x;
	float				step;
	float				tex_offset;
	float				fog;
	int					y_unclipped[2];

	y_unclipped[0] = rayhit->wall_bounds[0];
	y_unclipped[1] = rayhit->wall_bounds[1];
	step = (float)(y_unclipped[1] - y_unclipped[0] + 1);
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
	tex_x = get_tex_x(game, rayhit, &texture, &step);
	tex_offset = (y[0] - y_unclipped[0]) * step;
	fog = fog_factor(rayhit->distance);
	if (texture == game->cub_data.block.atlas.xpm)
		paint_vertical_line_texture_bonus(x, y, game, texture, tex_x,
			tex_offset, step, fog, &game->cub_data.block.anims[ANIM_BREATHE]);
	else if (texture == game->cub_data.effects.door_atlas.xpm)
		paint_vertical_line_texture_bonus(x, y, game, texture, tex_x,
			tex_offset, step, fog,
			&game->cub_data.effects.door_anims[ANIM_BREATHE]);
	else
		paint_vertical_line_texture_bonus(x, y, game, texture, tex_x,
			tex_offset, step, fog, NULL);
}
