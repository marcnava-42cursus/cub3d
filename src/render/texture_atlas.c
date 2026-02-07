/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_atlas.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/28 11:23:30 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/07 00:06:16 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

void	atlas_init(t_atlas *atlas, char *xpm_path, unsigned int frame_width,
		unsigned int frame_height)
{
	atlas->xpm = mlx_load_xpm42(xpm_path);
	atlas->frame_width = frame_width;
	atlas->frame_height = frame_height;
	atlas->max_frame[X] = atlas->xpm->texture.width / atlas->frame_width;
	atlas->max_frame[Y] = atlas->xpm->texture.height / atlas->frame_height;
	atlas->total_frames = atlas->max_frame[X] * atlas->max_frame[Y];
}

void	atlas_free(t_atlas *atlas)
{
	if (atlas->xpm != NULL)
	{
		mlx_delete_xpm42(atlas->xpm);
		atlas->xpm = NULL;
	}
}
