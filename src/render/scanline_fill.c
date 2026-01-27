/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanline_fill.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 21:56:23 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/27 19:12:13 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static void	sort_vertices(vertex_t *v1, vertex_t *v2, vertex_t *v3)
{
	vertex_t	temp_vertex;

	if (v1->y > v2->y)
	{
		temp_vertex = *v1;
		*v1 = *v2;
		*v2 = temp_vertex;
	}
	if (v2->y > v3->y)
	{
		temp_vertex = *v2;
		*v2 = *v3;
		*v3 = temp_vertex;
	}
	if (v1->y > v2->y)
	{
		temp_vertex = *v1;
		*v1 = *v2;
		*v2 = temp_vertex;
	}
}

static float	find_intersections(int y, vertex_t *first, vertex_t *second)
{
	float	t;	

	if (second->y != first->y)
	{
		t = (y - first->y) / (second->y - first->y);
		return (first->x + t * (second->x - first->x));
	}
	else
		return (first->x);
}

static void	check_and_swap_x(float *x_left, float *x_right)
{
	float		temp_float;

	if (*x_left > *x_right)
	{
		temp_float = *x_left;
		*x_left = *x_right;
		*x_right = temp_float;
	}
}

static void	fill_scanline(mlx_image_t *img, int y, int x_left, int x_right)
{
	int	pixel[2];

	pixel[X] = x_left;
	pixel[Y] = y;
	while (pixel[X] <= x_right)
	{
		if (is_inside_minimap_circle(pixel[X], pixel[Y]))
			safe_put_pixel(img, pixel[X], pixel[Y], MEDIUM_GREY);
		pixel[X]++;
	}
}

void	fill_triangle_scanline(mlx_image_t *img, vertex_t v1, vertex_t v2,
		vertex_t v3)
{
	int		y;
	float	x_left;
	float	x_right;

	sort_vertices(&v1, &v2, &v3);
	y = (int)v1.y;
	while (y <= (int)v3.y)
	{
		if (y < 0 || y >= (int)img->height)
		{
			y++;
			continue ;
		}
		x_right = find_intersections(y, &v1, &v3);
		if (y <= v2.y)
			x_left = find_intersections(y, &v1, &v2);
		else
			x_left = find_intersections(y, &v2, &v3);
		check_and_swap_x(&x_left, &x_right);
		fill_scanline(img, y, (int)x_left, (int)x_right);
		y++;
	}
}
