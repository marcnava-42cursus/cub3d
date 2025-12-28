/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 17:23:22 by ivmirand          #+#    #+#             */
/*   Updated: 2025/12/27 23:57:13 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

bool	is_inside_minimap_circle(int x, int y)
{
	int	center[2];
	int	d[2];
	int	radius;

	center[0] = MINIMAP_WIDTH / 2;
	center[1] = MINIMAP_HEIGHT / 2;
	d[0] = x - center[0];
	d[1] = y - center[1];
	radius = (MINIMAP_DIAMETER * MINIMAP_TILE_SIZE) / 2;
	return (d[0] * d[0] + d[1] * d[1] <= radius * radius);
}

// Rotate point around origin by given angle (counter-clockwise)
vertex_t	rotate_point(vertex_t point, float angle)
{
	vertex_t	rotated;
	float		cos_a;
	float		sin_a;

	cos_a = cosf(angle);
	sin_a = sinf(angle);
	rotated.x = point.x * cos_a - point.y * sin_a;
	rotated.y = point.x * sin_a + point.y * cos_a;
	return (rotated);
}

// Clipped version of bresenham that only draws within the circle
// Use normal bresenham but check each pixel
void	bresenham_clipped(vertex_t *start, vertex_t *end, mlx_image_t *img,
		int color)
{
	int	x0;
	int	y0;
	int	x1;
	int	y1;
	int	dx;
	int	dy;
	int	sx;
	int	sy;
	int	err;
	int	e2;

	x0 = (int)start->x;
	y0 = (int)start->y;
	x1 = (int)end->x;
	y1 = (int)end->y;
	dx = abs(x1 - x0);
	dy = abs(y1 - y0);
	if (x0 < x1)
		sx = 1;
	else
		sx = -1;
	if (y0 < y1)
		sy = 1;
	else
		sy = -1;
	err = dx - dy;
	while (true)
	{
	//	if (is_inside_minimap_circle(x0, y0))
	//		safe_put_pixel(img, x0, y0, color);
		if (x0 >= 0 && x0 < (int)img->width && y0 >= 0
			&& y0 < (int)img->height && is_inside_minimap_circle(x0, y0))
			save_pixel_to_image(img, x0, y0, color);
		if (x0 == x1 && y0 == y1)
			break ;
		e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}

// Convert world coordinates to rotated minimap coordinates
vertex_t	world_to_minimap_vertex(t_minimap *minimap, vertex_t world)
{
	vertex_t	world_offset;
	vertex_t	rotated_offset;
	vertex_t	minimap_vertex;
	float		scale;

	world_offset.x = world.x - (minimap->player->x + 0.25f)
		* WORLDMAP_TILE_SIZE;
	world_offset.y = world.y - (minimap->player->y + 0.25f)
		* WORLDMAP_TILE_SIZE;
	rotated_offset = rotate_point(world_offset,
			-(minimap->player->angle + FT_PI / 2));
	scale = (float)MINIMAP_TILE_SIZE / WORLDMAP_TILE_SIZE;
	minimap_vertex.x = (MINIMAP_WIDTH / 2) + rotated_offset.x * scale;
	minimap_vertex.y = (MINIMAP_HEIGHT / 2) + rotated_offset.y * scale;
	return (minimap_vertex);
}
