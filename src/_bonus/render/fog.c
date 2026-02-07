/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fog.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 14:53:54 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/07 01:06:44 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_bonus.h"

float	fog_factor(float distance)
{
	float	start;
	float	end;
	float	range;

	start = FOG_TILE_START * WORLDMAP_TILE_SIZE;
	end = FOG_TILE_END * WORLDMAP_TILE_SIZE;
	range = (distance - start) / (end - start);
	range = clamp(range, 0.0f, 1.0f);
	range *= range;
	return (range);
}

//shouldn't lerp the alpha values
//color_new[3] = rgba_color_1[3] + (rgba_color_2[3] - rgba_color_1[3]) * t;	
uint32_t	rgba_color_lerp(uint32_t color_1, uint32_t color_2, float t)
{
	uint8_t	rgba_color_1[4];
	uint8_t	rgba_color_2[4];
	uint8_t	color_new[4];

	rgba_color_1[0] = (color_1 >> 24) & 0xFF;
	rgba_color_2[0] = (color_2 >> 24) & 0xFF;
	rgba_color_1[1] = (color_1 >> 16) & 0xFF;
	rgba_color_2[1] = (color_2 >> 16) & 0xFF;
	rgba_color_1[2] = (color_1 >> 8) & 0xFF;
	rgba_color_2[2] = (color_2 >> 8) & 0xFF;
	rgba_color_1[3] = color_1 & 0xFF;
	rgba_color_2[3] = color_2 & 0xFF;
	color_new[0] = rgba_color_1[0] + (rgba_color_2[0] - rgba_color_1[0]) * t;
	color_new[1] = rgba_color_1[1] + (rgba_color_2[1] - rgba_color_1[1]) * t;
	color_new[2] = rgba_color_1[2] + (rgba_color_2[2] - rgba_color_1[2]) * t;
	color_new[3] = rgba_color_1[3];
	return ((color_new[0] << 24)
		| (color_new[1] << 16)
		| (color_new[2] << 8)
		| color_new[3]);
}
