/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bresenham_clipped.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 20:29:18 by ivmirand          #+#    #+#             */
/*   Updated: 2026/02/07 01:09:32 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_bonus.h"

static int	init_bresenham(int start_end[4], int dist_step[4], vertex_t *start,
		vertex_t *end)
{
	start_end[X] = (int)start->x;
	start_end[Y] = (int)start->y;
	start_end[U] = (int)end->x;
	start_end[V] = (int)end->y;
	dist_step[X] = abs(start_end[U] - start_end[X]);
	dist_step[Y] = abs(start_end[V] - start_end[Y]);
	if (start_end[X] < start_end[U])
		dist_step[U] = 1;
	else
		dist_step[U] = -1;
	if (start_end[X] < start_end[V])
		dist_step[V] = 1;
	else
		dist_step[V] = -1;
	return (dist_step[X] - dist_step[Y]);
}

// Clipped version of bresenham that only draws within the circle
// Use normal bresenham but check each pixel
void	bresenham_clipped(vertex_t *start, vertex_t *end, mlx_image_t *img,
		int color)
{
	int	start_end[4];
	int	dist_step[4];
	int	bresenham[2];

	bresenham[0] = init_bresenham(start_end, dist_step, start, end);
	while (true)
	{
		if (is_inside_minimap_circle(start_end[X], start_end[Y]))
			safe_put_pixel(img, start_end[X], start_end[Y], color);
		if (start_end[X] == start_end[U] && start_end[Y] == start_end[V])
			break ;
		bresenham[1] = bresenham[0] * 2;
		if (-dist_step[Y] < bresenham[1])
		{
			bresenham[0] -= dist_step[Y];
			start_end[X] += dist_step[U];
		}
		if (dist_step[X] > bresenham[1])
		{
			bresenham[0] += dist_step[X];
			start_end[Y] += dist_step[V];
		}
	}
}
