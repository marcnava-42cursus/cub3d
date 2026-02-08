/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paint_orb_by_line.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 10:55:04 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/08 13:24:28 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_bonus.h"

void	pack_x_y_w_to_orb_line(t_orb_line *orb_line, int draw_x[2],
		int draw_y[2], int sprite_width)
{
	orb_line->draw_x[0] = draw_x[0];
	orb_line->draw_x[1] = draw_x[1];
	orb_line->draw_y[0] = draw_y[0];
	orb_line->draw_y[1] = draw_y[1];
	orb_line->sprite_width = sprite_width;
}

void	pack_cam_y_and_scrn_xos_to_orb_line(t_orb_line *orb_line, float cam_y,
		float screen_x_o_s[3])
{
	orb_line->cam_y = cam_y;
	orb_line->screen_x_o_s[0] = screen_x_o_s[0];
	orb_line->screen_x_o_s[1] = screen_x_o_s[1];
	orb_line->screen_x_o_s[2] = screen_x_o_s[2];
}

static float	set_float_x(t_orb_line *orb_line, int x, int frame_width)
{
	float	float_x;

	float_x = ((x - (orb_line->screen_x_o_s[0] - orb_line->sprite_width / 2))
			* frame_width / orb_line->sprite_width) - 1;
	float_x = clamp(float_x, 0.0f, (float)(frame_width - 1));
	return (float_x);
}

void	paint_orb_by_line(t_game *game, t_rayhit *rayhits, t_orb_line *orb_line)
{
	float		x_offset_step[3];
	int			frame_width;
	int			x_y_packed[3];
	t_vert_line	vert_line;

	pack_x_ys(orb_line->draw_x[0], orb_line->draw_y, x_y_packed);
	frame_width = game->cub_data.effects.orb_atlas.frame_width;
	while (x_y_packed[0] < orb_line->draw_x[1])
	{
		x_offset_step[0] = set_float_x(orb_line, x_y_packed[0], frame_width);
		if (orb_line->cam_y < rayhits[x_y_packed[0]].distance)
		{
			x_offset_step[1] = orb_line->screen_x_o_s[1];
			x_offset_step[2] = orb_line->screen_x_o_s[2];
			pack_tex_and_anim_for_vert_line(game->cub_data.effects.orb_atlas
				.xpm, &game->cub_data.effects.orb_anims[
				game->cub_data.effects.current_orb_anim], &vert_line);
			pack_coords_and_fog_for_vert_line(x_y_packed, x_offset_step,
				fog_factor(orb_line->cam_y), &vert_line);
			paint_vertical_line_texture_bonus(game, &vert_line);
		}
		x_y_packed[0]++;
	}
}
