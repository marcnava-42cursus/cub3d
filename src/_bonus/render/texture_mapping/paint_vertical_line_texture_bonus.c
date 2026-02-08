/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paint_vertical_line_texture_bonus.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 13:15:12 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_bonus.h"
#include "animation.h"

void	pack_coords_and_fog_for_vert_line(int x_y_packed[3],
		float x_offset_step[3], float fog, t_vert_line *vert_line)
{
	vert_line->x_y_packed[0] = x_y_packed[0];
	vert_line->x_y_packed[1] = x_y_packed[1];
	vert_line->x_y_packed[2] = x_y_packed[2];
	vert_line->x_offset_step[0] = x_offset_step[0];
	vert_line->x_offset_step[1] = x_offset_step[1];
	vert_line->x_offset_step[2] = x_offset_step[2];
	vert_line->fog = fog;
}

void	pack_tex_and_anim_for_vert_line(xpm_t *texture, t_anim *anim,
		t_vert_line *vert_line)
{
	vert_line->texture = texture;
	vert_line->anim = anim;
}

// Textured vertical line painter - y[0] is start and y[1] is end
void	paint_vertical_line_texture_bonus(t_game *game, t_vert_line *vert_line)
{
	int			pixel[2];
	int			tex_coord[2];
	uint32_t	pixel_color;

	pixel[X] = vert_line->x_y_packed[0];
	pixel[Y] = vert_line->x_y_packed[1];
	tex_coord[X] = (int)vert_line->x_offset_step[0];
	while (pixel[Y] <= vert_line->x_y_packed[2])
	{
		tex_coord[Y] = (int)vert_line->x_offset_step[1];
		pixel_color = get_pixel_color_bonus(vert_line->anim, vert_line->texture,
				tex_coord);
		paint_pixel_color_bonus(game, pixel, pixel_color,
			vert_line->fog);
		pixel[Y]++;
		vert_line->x_offset_step[1] += vert_line->x_offset_step[2];
	}
}
