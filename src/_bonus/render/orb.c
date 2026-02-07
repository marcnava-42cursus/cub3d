/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orb.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 10:55:04 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/07 21:18:23 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_bonus.h"

static bool	set_cam_x_y(t_game *game, float cam[2], float ray_dir[4])
{
	float	dir[2];
	float	plane[2];
	float	rel[2];
	float	inv_det;

	dir[X] = (ray_dir[X] + ray_dir[U]) * 0.5f;
	dir[Y] = (ray_dir[Y] + ray_dir[V]) * 0.5f;
	plane[X] = (ray_dir[U] - ray_dir[X]) * 0.5f;
	plane[Y] = (ray_dir[V] - ray_dir[Y]) * 0.5f;
	rel[X] = game->orb.x - game->cub_data.player.x;
	rel[Y] = game->orb.y - game->cub_data.player.y;
	inv_det = 1.0f / (plane[X] * dir[Y] - dir[X] * plane[Y]);
	cam[X] = inv_det * (dir[Y] * rel[X] - dir[X] * rel[Y]);
	cam[Y] = inv_det * (-plane[Y] * rel[X] + plane[X] * rel[Y]);
	if (cam[Y] <= 0.01f)
		return (false);
	return (true);
}

static void	set_draw_x_and_draw_y(int draw_x[2], int draw_y[2], float center,
		int sprite_render_dims[2], float *tex_step,
		float *tex_pos_start, t_game *game, int screen_x)
{
	float	sprite_top;

	sprite_top = center - sprite_render_dims[Y] * 0.5f;
	draw_y[0] = (int)sprite_top;
	draw_y[1] = (int)(center + sprite_render_dims[Y] * 0.5f);
	if (draw_y[0] < 0)
		draw_y[0] = 0;
	if (draw_y[1] >= (int)game->double_buffer[NEXT]->height)
		draw_y[1] = (int)game->double_buffer[NEXT]->height - 1;
	*tex_step = (float)game->cub_data.effects.orb_atlas.frame_height
		/ (float)sprite_render_dims[Y];
	*tex_pos_start = (draw_y[0] - sprite_top) * *tex_step;
	if (*tex_pos_start < 0.0f)
		*tex_pos_start = 0.0f;
	draw_x[0] = screen_x - sprite_render_dims[X] / 2;
	draw_x[1] = screen_x + sprite_render_dims[X] / 2;
	if (draw_x[1] < 0 || draw_x[0] >= (int)game->double_buffer[NEXT]->width)
		return ;
	if (draw_x[0] < 0)
		draw_x[0] = 0;
	if (draw_x[1] >= (int)game->double_buffer[NEXT]->width)
		draw_x[1] = (int)game->double_buffer[NEXT]->width - 1;
}

static void	paint_orb_by_line(t_game *game, int draw_x[2], int draw_y[2],
		int screen_x, int sprite_width, float cam_y, t_rayhit *rayhits,
		float tex_pos_start, float tex_step)
{
	int		tex_x;
	float	fog;
	float	x_offset_step[3];
	int		frame_width;
	int		x_y_packed[3];

	pack_x_ys(draw_x[0], draw_y, x_y_packed);
	frame_width = game->cub_data.effects.orb_atlas.frame_width;
	while (x_y_packed[0] < draw_x[1])
	{
		tex_x = (int)((x_y_packed[0] - (screen_x - sprite_width / 2))
				* frame_width / sprite_width) - 1;
		tex_x = (int)clamp((float)tex_x, 0.0f, (float)(frame_width - 1));
		if (cam_y < rayhits[x_y_packed[0]].distance)
		{
			fog = fog_factor(cam_y);
			x_offset_step[0] = (float)tex_x;
			x_offset_step[1] = tex_pos_start;
			x_offset_step[2] = tex_step;
			paint_vertical_line_texture_bonus(x_y_packed, game,
				game->cub_data.effects.orb_atlas.xpm,
				x_offset_step, fog, &game->cub_data.effects.orb_anims[
				game->cub_data.effects.current_orb_anim]);
		}
		x_y_packed[0]++;
	}
}

void	render_orb(t_game *game, t_rayhit *rayhits, float center,
		float ray_dir[4])
{
	float	cam[2];
	int		screen_x;
	int		sprite_render_dims[2];
	int		draw_y[2];
	float	tex_pos_start;
	float	tex_step;
	int		draw_x[2];

	if (!game->orb.active || game->orb.mode == ORB_MODE_PLACE
		|| !set_cam_x_y(game, cam, ray_dir))
		return ;
	screen_x = (int)(game->double_buffer[NEXT]->width / 2)
		* (1 + cam[X] / cam[Y]);
	sprite_render_dims[Y] = (game->double_buffer[NEXT]->height / cam[Y]) / 2;
	sprite_render_dims[X] = sprite_render_dims[Y];
	if (sprite_render_dims[Y] == 0)
		return ;
	set_draw_x_and_draw_y(draw_x, draw_y, center, sprite_render_dims,
		&tex_step, &tex_pos_start, game, screen_x);
	paint_orb_by_line(game, draw_x, draw_y, screen_x, sprite_render_dims[X],
		cam[Y] * WORLDMAP_TILE_SIZE, rayhits, tex_pos_start, tex_step);
}
