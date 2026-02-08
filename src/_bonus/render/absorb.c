/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   absorb.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 19:34:24 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/08 14:10:16 by ivmirand         ###   ########.fr       */
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

static void	pack_x_offset_step(float x_offset_step[3], t_rayhit *rayhit,
		t_game *game, int x_y_packed[3])
{
	int				y[2];

	y[0] = rayhit->wall_bounds[0];
	y[1] = rayhit->wall_bounds[1];
	if (y[0] < 0)
		y[0] = 0;
	if (y[1] >= (int)game->double_buffer[NEXT]->height)
		y[1] = (int)game->double_buffer[NEXT]->height - 1;
	if (y[0] >= y[1])
		return ;
	x_offset_step[0] = (float)get_effects_tex_x(&game->cub_data.effects,
			rayhit);
	x_offset_step[2] = (float)game->cub_data.effects.absorb_atlas
		.frame_height / (float)(rayhit->wall_bounds[1] - rayhit->wall_bounds[0]
			+ 1);
	x_offset_step[1] = (y[0] - rayhit->wall_bounds[0]) * x_offset_step[2];
	pack_x_ys(0, y, x_y_packed);
}

static void	lerp(t_game *game, int x_y_packed[3], float x_offset_step[3],
		float fog)
{
	int			pixel[2];
	int			tex_coord[2];
	uint32_t	pixel_color;

	pixel[X] = x_y_packed[0];
	pixel[Y] = x_y_packed[1];
	tex_coord[X] = (int)x_offset_step[0];
	while (pixel[Y] <= x_y_packed[2])
	{
		tex_coord[Y] = (int)x_offset_step[1];
		pixel_color = get_pixel_color_bonus(
				&game->cub_data.effects.absorb_anims[
				game->cub_data.effects.current_absorb_anim],
				game->cub_data.effects.absorb_atlas.xpm, tex_coord);
		pixel_color = rgba_color_lerp(game->double_buffer[NEXT]->pixels[
				(pixel[Y] * game->double_buffer[NEXT]->width + pixel[X]) * 4],
				pixel_color, 0.5f);
		paint_pixel_color_bonus(game, pixel, pixel_color, fog);
		pixel[Y]++;
		x_offset_step[1] += x_offset_step[2];
	}
}

void	render_absorb_effects(t_game *game, t_rayhit *rh)
{
	unsigned int	i;
	int				c_cell[2];
	t_orientation	c_face;
	float			x_offset_step[3];
	int				x_y_packed[3];

	i = 0;
	c_cell[X] = rh[game->double_buffer[NEXT]->width / 2].cell[X];
	c_cell[Y] = rh[game->double_buffer[NEXT]->width / 2].cell[Y];
	c_face = rh[game->double_buffer[NEXT]->width / 2].face;
	if (game->cub_data.player.inventory == '\0'
		&& game->cub_data.map.grid[c_cell[Y]][c_cell[X]] != '2')
		return ;
	while (i < game->double_buffer[NEXT]->width && i < MAX_WINDOW_WIDTH)
	{
		if (rh[i].cell[X] == c_cell[X] && rh[i].cell[Y] == c_cell[Y]
			&& rh[i].face == c_face && rh[game->double_buffer[NEXT]->width / 2]
			.distance <= 300.0f)
		{
			pack_x_offset_step(x_offset_step, &rh[i], game, x_y_packed);
			x_y_packed[0] = i;
			lerp(game, x_y_packed, x_offset_step, fog_factor(rh[i].distance));
		}
		i++;
	}
}
