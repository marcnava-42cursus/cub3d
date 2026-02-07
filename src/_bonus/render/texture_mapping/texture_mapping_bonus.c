/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_mapping_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/07 15:17:22 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_bonus.h"
#include "animation.h"

static xpm_t *get_texture_from_cell(t_game *game, t_rayhit *rayhit, int source_wh[2])
{
	char	cell_char;
	xpm_t	*texture;

	texture = NULL;
	cell_char = game->cub_data.map.grid[rayhit->cell[Y]][rayhit->cell[X]];
	if (rayhit->face == NORTH)
		texture = game->cub_data.textures.north;
	else if (rayhit->face == SOUTH)
		texture = game->cub_data.textures.south;
	else if (rayhit->face == EAST)
		texture = game->cub_data.textures.east;
	else if (rayhit->face == WEST)
		texture = game->cub_data.textures.west;
	if (cell_char == '2')
		texture = game->cub_data.block.atlas.xpm;
	else if (ft_strchr("!\"·$%&/()=?", cell_char))
		texture = game->cub_data.effects.door_atlas.xpm;
	if (texture == game->cub_data.block.atlas.xpm)
	{
		source_wh[0] = game->cub_data.block.atlas.frame_width;
		source_wh[1] = game->cub_data.block.atlas.frame_height;
	}
	else if (texture == game->cub_data.effects.door_atlas.xpm)
	{
		source_wh[0] = game->cub_data.effects.door_atlas.frame_width;
		source_wh[1] = game->cub_data.effects.door_atlas.frame_height;
	}
	else if (texture)
	{
		source_wh[0] = texture->texture.width;
		source_wh[1] = texture->texture.height;
	}
	return(texture);
}

static int	get_tex_x(t_game *game, t_rayhit *rayhit, xpm_t **texture,
		float *original_line_height)
{
	int		tex_x;
	float	wall_x;
	int		source_wh[2];

	*texture = get_texture_from_cell(game, rayhit, source_wh);
	if (!texture)
		return (-1);
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
	int			pixel[2];
	int			tex_coord[2];
	uint32_t	pixel_color;

	pixel[X] = (int)x;
	pixel[Y] = y[0];
	tex_coord[X] = tex_x;
	while (pixel[Y] <= y[1])
	{
		tex_coord[Y] = (int)tex_pos;
		pixel_color = get_pixel_color_bonus(anim, texture, tex_coord);
		paint_pixel_color_bonus(game, pixel, pixel_color, fog);
		pixel[Y]++;
		tex_pos += tex_step;
	}
}

void	render_texture_line_bonus(t_rayhit *rayhit, unsigned int x,
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
	if (x >= game->double_buffer[NEXT]->width)
		return ;
	if (y_unclipped[0] >= y_unclipped[1])
		return ;
	tex_x = get_tex_x(game, rayhit, &texture, &step);
	if (tex_x == -1)
		return ;
	tex_offset = (rayhit->wall_bounds[0] - y_unclipped[0]) * step;
	fog = fog_factor(rayhit->distance);
	if (texture == game->cub_data.block.atlas.xpm)
		paint_vertical_line_texture_bonus(x, rayhit->wall_bounds, game, texture,
			tex_x, tex_offset, step, fog,
			&game->cub_data.block.anims[ANIM_BREATHE]);
	else if (texture == game->cub_data.effects.door_atlas.xpm)
		paint_vertical_line_texture_bonus(x, rayhit->wall_bounds, game, texture,
			tex_x, tex_offset, step, fog,
			&game->cub_data.effects.door_anims[
			game->cub_data.effects.current_door_anim]);
	else
		paint_vertical_line_texture_bonus(x, rayhit->wall_bounds, game, texture,
			tex_x, tex_offset, step, fog, NULL);
}
