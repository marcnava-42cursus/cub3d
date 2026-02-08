/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_mapping_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 13:22:33 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_bonus.h"
#include "animation.h"

static xpm_t	*get_texture_from_rayhit(t_game *game, t_rayhit *rayhit)
{
	xpm_t	*texture;

	texture = NULL;
	if (rayhit->face == NORTH)
		texture = game->cub_data.textures.north;
	else if (rayhit->face == SOUTH)
		texture = game->cub_data.textures.south;
	else if (rayhit->face == EAST)
		texture = game->cub_data.textures.east;
	else if (rayhit->face == WEST)
		texture = game->cub_data.textures.west;
	return (texture);
}

static xpm_t	*get_texture_from_cell(t_game *game, t_rayhit *rayhit,
		int source_wh[2])
{
	char	cell_char;
	xpm_t	*texture;

	texture = get_texture_from_rayhit(game, rayhit);
	cell_char = game->cub_data.map.grid[rayhit->cell[Y]][rayhit->cell[X]];
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
	return (texture);
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
	if ((rayhit->side == 0 && rayhit->step[X] < 0)
		|| (rayhit->side == 1 && rayhit->step[Y] > 0))
		tex_x = source_wh[0] - tex_x - 1;
	*original_line_height = (float)source_wh[1] / *original_line_height;
	return (tex_x);
}

static xpm_t	*get_x_offset_step(float x_offset_step[3], int x_y_packed[3],
		t_game *game, t_rayhit *rayhit)
{
	xpm_t	*texture;

	texture = NULL;
	x_offset_step[2] = (float)(x_y_packed[2] - x_y_packed[1] + 1);
	if ((unsigned int)x_y_packed[0] >= game->double_buffer[NEXT]->width)
		return (NULL);
	if (x_y_packed[1] >= x_y_packed[2])
		return (NULL);
	x_offset_step[0] = (float)get_tex_x(game, rayhit, &texture,
			&x_offset_step[2]);
	if (x_offset_step[0] == -1.0f)
		return (NULL);
	x_offset_step[1] = (rayhit->wall_bounds[0] - x_y_packed[1])
		* x_offset_step[2];
	return (texture);
}

void	render_texture_line_bonus(t_rayhit *rayhit, unsigned int x,
			t_game *game)
{
	xpm_t		*texture;
	float		x_offset_step[3];
	int			x_y_packed[3];
	t_vert_line	vert_line;

	pack_x_ys((int)x, rayhit->wall_bounds, x_y_packed);
	texture = get_x_offset_step(x_offset_step, x_y_packed, game, rayhit);
	if (!texture)
		return ;
	pack_tex_and_anim_for_vert_line(texture, NULL, &vert_line);
	pack_coords_and_fog_for_vert_line(x_y_packed, x_offset_step,
		fog_factor(rayhit->distance), &vert_line);
	if (texture == game->cub_data.block.atlas.xpm)
	{
		vert_line.anim = &game->cub_data.block.anims[ANIM_BREATHE];
		paint_vertical_line_texture_bonus(game, &vert_line);
	}
	else if (texture == game->cub_data.effects.door_atlas.xpm)
	{
		vert_line.anim = &game->cub_data.effects.door_anims[
			game->cub_data.effects.current_door_anim];
		paint_vertical_line_texture_bonus(game, &vert_line);
	}
	else if (game->cub_data.map.grid[rayhit->cell[Y]][rayhit->cell[X]] != '3')
		paint_vertical_line_texture_bonus(game, &vert_line);
}
