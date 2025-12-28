/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_atlas.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/28 11:23:30 by ivmirand          #+#    #+#             */
/*   Updated: 2025/12/28 12:49:13 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "animation.h"

static void	set_max_frames(t_atlas *atlas)
{
	unsigned int	x_frames;
	unsigned int	y_frames;

	x_frames = atlas->xpm->texture.width / atlas->frame_width;
	y_frames = atlas->xpm->texture.height / atlas->frame_height;
	atlas->max_frames = x_frames * y_frames;
}

static void	set_total_frames(t_atlas *atlas)
{
}

void	atlas_init(t_atlas *atlas, char *xpm_path, unsigned int frame_width,
		unsigned int frame_height)
{
	atlas->xpm_path = xpm_path;
	atlas->frame_width = frame_width;
	atlas->frame_height = frame_height;
	set_max_frames(atlas);
	set_total_frames(atlas);
	atlas->current_frame = 0;
}
