/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 17:23:22 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/07 01:05:48 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_bonus.h"

bool	is_inside_minimap_circle(int coord_x, int coord_y)
{
	int	center[2];
	int	d[2];
	int	radius;

	center[X] = MINIMAP_WIDTH / 2;
	center[Y] = MINIMAP_HEIGHT / 2;
	d[X] = coord_x - center[X];
	d[Y] = coord_y - center[Y];
	radius = (MINIMAP_DIAMETER * MINIMAP_TILE_SIZE) / 2;
	return (d[X] * d[X] + d[Y] * d[Y] <= radius * radius);
}

// Rotate point around origin by given angle (counter-clockwise)
vertex_t	rotate_point(float x, float y, float angle)
{
	vertex_t	rotated;
	float		cos_a;
	float		sin_a;

	cos_a = cosf(angle);
	sin_a = sinf(angle);
	rotated.x = x * cos_a - y * sin_a;
	rotated.y = x * sin_a + y * cos_a;
	return (rotated);
}

// Convert world coordinates to rotated minimap coordinates
vertex_t	world_to_minimap_vertex(t_minimap *minimap, vertex_t world)
{
	float		world_offset[2];
	vertex_t	rotated_offset;
	vertex_t	minimap_vertex;
	float		scale;

	world_offset[X] = world.x - (minimap->player->x) * WORLDMAP_TILE_SIZE;
	world_offset[Y] = world.y - (minimap->player->y) * WORLDMAP_TILE_SIZE;
	rotated_offset = rotate_point(world_offset[X], world_offset[Y],
			-(minimap->player->angle + FT_PI / 2));
	scale = (float)MINIMAP_TILE_SIZE / WORLDMAP_TILE_SIZE;
	minimap_vertex.x = (MINIMAP_WIDTH / 2) + rotated_offset.x * scale;
	minimap_vertex.y = (MINIMAP_HEIGHT / 2) + rotated_offset.y * scale;
	return (minimap_vertex);
}
