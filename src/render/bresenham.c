/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bresenham.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 20:29:18 by ivmirand          #+#    #+#             */
/*   Updated: 2025/12/02 16:15:53 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static void	safe_put_pixel(mlx_image_t *img, int x, int y, unsigned int color)
{
	if ((unsigned int)x < img->width && (unsigned int)y < img->height)
		save_pixel_to_image(img, x, y, color);
}

static int	calc_distance(int *d_x, int *d_y, vertex_t start, vertex_t end)
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

	yi = calc_distance((int *)&distance.x, (int *)&distance.y, start, end);
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

	xi = calc_distance((int *)&distance.x, (int *)&distance.y, start, end);
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
