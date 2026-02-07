/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   absorb.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 19:34:24 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/07 01:06:54 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_bonus.h"

static int	get_effects_tex_x(t_effects *effects, t_rayhit *rayhit)
{
	float	wall_x;
	int		frame_width;

	frame_width = effects->absorb_atlas.frame_width;
	if (rayhit->side == 0)
		wall_x = rayhit->position.y / WORLDMAP_TILE_SIZE;
	else
		wall_x = rayhit->position.x / WORLDMAP_TILE_SIZE;
	wall_x = wall_x - floorf(wall_x);
	return ((int)(wall_x * (float)frame_width));
}

void	render_absorb_effects(t_game *game, t_rayhit *rh, float center,
		float dist_to_proj_plane)
{
	unsigned int	i;
	int				c_cell[2];
	t_orientation	c_face;
	float			fog;
	float			step;
	float			tex_offset;
	int				y[2];
	int				tex_x;

	i = 0;
	c_cell[X] = rh[game->double_buffer[NEXT]->width / 2].cell[X];
	c_cell[Y] = rh[game->double_buffer[NEXT]->width / 2].cell[Y];
	c_face = rh[game->double_buffer[NEXT]->width / 2].face;
	if (game->cub_data.player.inventory == '\0')
	{
		if (game->cub_data.map.grid[c_cell[Y]][c_cell[X]] != '2')
			return ;
	}
	while (i < game->double_buffer[NEXT]->width && i < MAX_WINDOW_WIDTH)
	{
		if (rh[i].cell[X] == c_cell[X] && rh[i].cell[Y] == c_cell[Y]
			&& rh[i].face == c_face && rh[game->double_buffer[NEXT]->width / 2]
				.distance <= 300.0f)
		{
			y[0] = rh[i].wall_bounds[0];
			y[1] = rh[i].wall_bounds[1];
			if (y[0] < 0)
				y[0] = 0;
			if (y[1] >= (int)game->double_buffer[NEXT]->height)
				y[1] = (int)game->double_buffer[NEXT]->height - 1;
			if (y[0] >= y[1])
				return ;
			tex_x = get_effects_tex_x(&game->cub_data.effects, &rh[i]);
			fog = fog_factor(rh[i].distance);
			step = (float)game->cub_data.effects.absorb_atlas.frame_height
				/ (float)(rh[i].wall_bounds[1] - rh[i].wall_bounds[0] + 1);
			tex_offset = (y[0] - rh[i].wall_bounds[0]) * step;
			paint_vertical_line_texture_bonus(i, y, game,
					game->cub_data.effects.absorb_atlas.xpm,
					tex_x, tex_offset, step, fog,
					&game->cub_data.effects.absorb_anims[
					game->cub_data.effects.current_absorb_anim]);	
		}
		i++;
	}
}

