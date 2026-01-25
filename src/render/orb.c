/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orb.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 10:55:04 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/26 00:48:46 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

void	render_orb(t_game *game, t_rayhit *rayhits, float center, float ray_dir[4])
{
	int				i;
	float			rel[2];
	float			cam[2];
	float			dir[2];
	float			plane[2];
	float			inv_det;
	int				screen_x;
	int				sprite_height;
	int				sprite_width;
	int				y[2];
	float			tex_pos_start;
	float			tex_step;
	int				draw_start;
	int				draw_end;
	float			fog;

	if (!game->orb.active)
		return ;
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
		return ;
	screen_x = (int)(game->double_buffer[NEXT]->width / 2) * (1 + cam[X] / cam[Y]);
	sprite_height = (game->double_buffer[NEXT]->height / cam[Y]) / 2;
	sprite_width = sprite_height;
	y[0] = center - sprite_height / 2;
	y[1] = center + sprite_height / 2;
	if (y[0] < 0)
		y[0] = 0;
	if (y[1] >= (int)game->double_buffer[NEXT]->height)
		y[1] = (int)game->double_buffer[NEXT]->height - 1;
	tex_step = (float)game->cub_data.player.textures.weapon.frame_height / (float)sprite_height;
	tex_pos_start = (y[0] - (center - sprite_height / 2.0f)) * tex_step;
	draw_start = screen_x - sprite_width / 2;
	draw_end = screen_x + sprite_width / 2;
	if (draw_end < 0 || draw_start >= (int)game->double_buffer[NEXT]->width)
		return ;
	if (draw_start < 0)
		draw_start = 0;
	if (draw_end >= (int)game->double_buffer[NEXT]->width)
		draw_end = (int)game->double_buffer[NEXT]->width - 1;
	i = draw_start;
	while (i < draw_end)
	{
		int tex_x;

		tex_x = (int)((i - (screen_x - sprite_width / 2))
					* game->cub_data.player.textures.weapon.frame_width
					/ sprite_width);
		tex_x = (int)clamp((float)tex_x, 0.0f, game->cub_data.player.textures.weapon.frame_width);
		if (cam[Y] < rayhits[i].distance)
		{
			fog = fog_factor(cam[Y]);
			fog = fog * fog;
			paint_vertical_line_texture(i, y, game->double_buffer[NEXT],
			game->cub_data.player.textures.weapon.xpm, tex_x, tex_pos_start,
			tex_step, fog);
		}
		i++;
	}
}
