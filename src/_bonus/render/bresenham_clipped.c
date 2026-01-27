/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bresenham_clipped.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 20:29:18 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/27 17:15:46 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static int	calc_increment(int *d_x, int *d_y, vertex_t start, vertex_t end)
{
	*d_x = end.x - start.x;
	*d_y = end.y - start.y;
	if (*d_y < *d_x)
	{
		if (*d_y < 0)
		{
			*d_y *= -1;
			return (-1);
		}
	}
	else
	{
		if (*d_x < 0)
		{
			*d_x *= -1;
			return (-1);
		}
	}
	return (1);
}

static void	paint_low(vertex_t start, vertex_t end, mlx_image_t *img, int color)
{
	vertex_t	distance;
	int			yi;
	int			bresenham;
	int			steps;
	vertex_t	vtx;

	yi = calc_increment((int *)&distance.x, (int *)&distance.y, start, end);
	bresenham = (2 * distance.y) - distance.x;
	vtx = start;
	vtx.v = fabsf(end.x - start.x);
	steps = 0;
	while (vtx.x != end.x && steps < distance.x + 1)
	{
		steps++;
		vtx.u = fabsf(vtx.x++ - start.x) / vtx.v;
		safe_put_pixel(img, vtx.x, vtx.y, color);
		if (bresenham > 0)
		{
			vtx.y += yi;
			bresenham += 2 * (distance.y - distance.x);
		}
		else
			bresenham += 2 * distance.y;
	}
}

static void	paint_high(vertex_t start, vertex_t end, mlx_image_t *img,
		int color)
{
	vertex_t	distance;
	int			xi;
	int			bresenham;
	int			steps;
	vertex_t	vtx;

	xi = calc_increment((int *)&distance.x, (int *)&distance.y, start, end);
	bresenham = (2 * distance.x) - distance.y;
	vtx = start;
	vtx.v = fabsf(end.y - start.y);
	steps = 0;
	while (vtx.y != end.y && steps < distance.y + 1)
	{
		steps++;
		vtx.u = fabsf(vtx.y++ - start.y) / vtx.v;
		safe_put_pixel(img, vtx.x, vtx.y, color);
		if (bresenham > 0)
		{
			vtx.x += xi;
			bresenham += 2 * (distance.x - distance.y);
		}
		else
			bresenham += 2 * distance.x;
	}
}

static int	init_bresenham(int start_end[4], int dist_step[4])
{
	start_end[X] = (int)start->x;
	start_end[Y] = (int)start->y;
	start_end[U] = (int)end->x;
	start_end[V] = (int)end->y;
	dist_step[X] = abs(start_end[U] - start_end[X]);
	dist_step[Y] = abs(start_end[V] - start_end[Y]);
	if (b_start[X] < b_end[X])
		dist_step[U] = 1;
	else
		dist_step[U] = -1;
	if (b_start[Y] < b_end[Y])
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
	int	bresenham;
	int	bresenham_times_two;

	bresenham = init_bresenham(start_end, dist_step);
	while (true)
	{
		if (is_inside_minimap_circle(b_start))
			safe_put_pixel(img, start_end[X], start_end[Y], color);
		if (start_end[X] == start_end[U] && start_end[Y] == start_end[V])
			break ;
		bresenham_times_two = bresenham * 2;
		if (-dist_step[Y] < bresenham_times_two)
		{
			bresenham -= dist_step[Y];
			start_end[X] += dist_step[U];
		}
		if (dist[X] > bresenham_times_two)
		{
			bresenham += dist_step[X];
			start_end[Y] += dist_step[V];
		}
	}
}
