/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orb.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 10:55:04 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/27 01:56:18 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

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
	draw_y[0] = center - sprite_render_dims[Y] / 2;
	draw_y[1] = center + sprite_render_dims[Y] / 2;
	if (draw_y[0] < 0)
		draw_y[0] = 0;
	if (draw_y[1] >= (int)game->double_buffer[NEXT]->height)
		draw_y[1] = (int)game->double_buffer[NEXT]->height - 1;
	*tex_step = (float)game->cub_data.player.textures.weapon.frame_height
		/ (float)sprite_render_dims[Y];
	*tex_pos_start = (draw_y[0] - (center - sprite_render_dims[Y] / 2.0f))
		* *tex_step;
	draw_x[0] = screen_x - sprite_render_dims[X] / 2;
	draw_x[1] = screen_x + sprite_render_dims[X] / 2;
	if (draw_x[1] < 0 || draw_x[0] >= (int)game->double_buffer[NEXT]->width)
		return ;
	if (draw_x[0] < 0)
		draw_x[0] = 0;
	if (draw_x[1] >= (int)game->double_buffer[NEXT]->width)
		draw_x[1] = (int)game->double_buffer[NEXT]->width - 1;
}

static void	paint_orb_by_line(t_atlas *orb_atlas, mlx_image_t *img,
		int draw_x[2], int draw_y[2], int screen_x, int sprite_width,
		float cam_y, t_rayhit *rayhits, float tex_pos_start, float tex_step)
{
	int		tex_x;
	int		i;
	float	fog;

	i = draw_x[0];
	while (i < draw_x[1])
	{
		tex_x = (int)((i - (screen_x - sprite_width / 2))
				* orb_atlas->frame_width / sprite_width);
		tex_x = (int)clamp((float)tex_x, 0.0f, orb_atlas->frame_width);
		if (cam_y < rayhits[i].distance)
		{
			fog = fog_factor(cam_y);
			fog = fog * fog;
			paint_vertical_line_texture(i, draw_y, img, orb_atlas->xpm, tex_x,
					tex_pos_start, tex_step, fog);
		}
		i++;
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

	if (!game->orb.active || !set_cam_x_y(game, cam, ray_dir))
		return ;
	screen_x = (int)(game->double_buffer[NEXT]->width / 2)
		* (1 + cam[X] / cam[Y]);
	sprite_render_dims[Y] = (game->double_buffer[NEXT]->height / cam[Y]) / 2;
	sprite_render_dims[X] = sprite_render_dims[Y];
	if (sprite_render_dims[Y] == 0)
		return ;
	set_draw_x_and_draw_y(draw_x, draw_y, center, sprite_render_dims,
			&tex_step, &tex_pos_start, game, screen_x);
	paint_orb_by_line(&game->cub_data.player.textures.weapon,
			game->double_buffer[NEXT], draw_x, draw_y, screen_x,
			sprite_render_dims[X], cam[Y], rayhits, tex_pos_start, tex_step);
}
