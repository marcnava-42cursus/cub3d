/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orb.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 10:55:04 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/08 13:45:49 by ivmirand         ###   ########.fr       */
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

static void	set_draw_y(int draw_y[2], float center, int sprite_render_dims[2],
		t_game *game)
{
	float	sprite_top;

	sprite_top = center - sprite_render_dims[Y] * 0.5f;
	draw_y[0] = (int)sprite_top;
	draw_y[1] = (int)(center + sprite_render_dims[Y] * 0.5f);
	if (draw_y[0] < 0)
		draw_y[0] = 0;
	if (draw_y[1] >= (int)game->double_buffer[NEXT]->height)
		draw_y[1] = (int)game->double_buffer[NEXT]->height - 1;
}

static void	set_draw_x(int draw_x[2], int screen_x, int sprite_render_dims[2],
		t_game *game)
{
	draw_x[0] = screen_x - sprite_render_dims[X] / 2;
	draw_x[1] = screen_x + sprite_render_dims[X] / 2;
	if (draw_x[1] < 0 || draw_x[0] >= (int)game->double_buffer[NEXT]->width)
		return ;
	if (draw_x[0] < 0)
		draw_x[0] = 0;
	if (draw_x[1] >= (int)game->double_buffer[NEXT]->width)
		draw_x[1] = (int)game->double_buffer[NEXT]->width - 1;
}

static bool	set_sprite_render_dims(int sprite_render_dims[2], t_game *game,
		float cam_y)
{
	sprite_render_dims[Y] = (game->double_buffer[NEXT]->height / cam_y) / 2;
	sprite_render_dims[X] = sprite_render_dims[Y];
	if (sprite_render_dims[Y] == 0)
		return (false);
	return (true);
}

void	render_orb(t_game *game, t_rayhit *rayhits, float center,
		float ray_dir[4])
{
	float		cam[2];
	int			sprite_render_dims[2];
	int			draw[2][2];
	float		x_offset_step[3];
	t_orb_line	orb_line;

	if (!game->orb.active || game->orb.mode == ORB_MODE_PLACE
		|| !set_cam_x_y(game, cam, ray_dir)
		|| !set_sprite_render_dims(sprite_render_dims, game, cam[Y]))
		return ;
	x_offset_step[0] = (game->double_buffer[NEXT]->width / 2)
		* (1 + cam[X] / cam[Y]);
	set_draw_y(draw[Y], center, sprite_render_dims, game);
	set_draw_x(draw[X], (int)x_offset_step[0], sprite_render_dims, game);
	x_offset_step[2] = (float)game->cub_data.effects.orb_atlas.frame_height
		/ (float)sprite_render_dims[Y];
	x_offset_step[1] = (draw[Y][0] - (center - sprite_render_dims[Y] * 0.5f))
		* x_offset_step[2];
	if (x_offset_step[1] < 0.0f)
		x_offset_step[1] = 0.0f;
	pack_x_y_w_to_orb_line(&orb_line, draw[X], draw[Y], sprite_render_dims[X]);
	pack_cam_y_and_scrn_xos_to_orb_line(&orb_line, cam[Y], x_offset_step);
	paint_orb_by_line(game, rayhits, &orb_line);
}
