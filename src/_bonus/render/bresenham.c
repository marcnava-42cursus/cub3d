/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bresenham.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 20:29:18 by ivmirand          #+#    #+#             */
/*   Updated: 2025/12/28 20:40:15 by ivmirand         ###   ########.fr       */
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

void	bresenham(vertex_t *start, vertex_t *end, mlx_image_t *img, int color)
{
	if (end == NULL)
		safe_put_pixel(img, start->x, start->y, color);
	else
	{
		if (fabsf(end->y - start->y) < fabsf(end->x - start->x))
		{
			if (start->x > end->x)
				paint_low(*end, *start, img, color);
			else
				paint_low(*start, *end, img, color);
		}
		else
		{
			if (start->y > end->y)
				paint_high(*end, *start, img, color);
			else
				paint_high(*start, *end, img, color);
		}
	}
}

// Clipped version of bresenham that only draws within the circle
// Use normal bresenham but check each pixel
void	bresenham_clipped(vertex_t *start, vertex_t *end, mlx_image_t *img,
		int color)
{
	int	b_start[2];
	int	b_end[2];
	int	dist[2];
	int	step[2];
	int	bresenham;
	int	bresenham_times_two;

	b_start[X] = (int)start->x;
	b_start[Y] = (int)start->y;
	b_end[X] = (int)end->x;
	b_end[Y] = (int)end->y;
	dist[X] = ft_abs(b_end[X] - b_start[X]);
	dist[Y] = ft_abs(b_end[Y] - b_start[Y]);
	if (b_start[X] < b_end[X])
		step[X] = 1;
	else
		step[X] = -1;
	if (b_start[Y] < b_end[Y])
		step[Y] = 1;
	else
		step[Y] = -1;
	bresenham = dist[X] - dist[Y];
	while (true)
	{
		if (is_inside_minimap_circle(b_start))
			safe_put_pixel(img, b_start[X], b_start[Y], color);
		if (b_start[X] == b_end[X] && b_start[Y] == b_end[Y])
			break ;
		bresenham_times_two = bresenham * 2;
		if (-dist[Y] < bresenham_times_two)
		{
			bresenham -= dist[Y];
			b_start[X] += step[X];
		}
		if (dist[X] > bresenham_times_two)
		{
			bresenham += dist[X];
			b_start[Y] += step[Y];
		}
	}
}
