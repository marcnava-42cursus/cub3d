/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   outlines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 13:35:49 by ivmirand          #+#    #+#             */
/*   Updated: 2025/09/19 15:27:31 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static bool find_neighbor_edge(t_rayhit *current, t_rayhit *neighbor)
{
	float	distance_threshold;

	distance_threshold = fmaxf(8.0f, current->distance * 0.03f);
	if (!neighbor->hit || current->cell_x != neighbor->cell_x
		|| current->cell_y != neighbor->cell_y
		|| current->side != neighbor->side
		|| fabsf(current->distance - neighbor->distance) > distance_threshold)
			return (true);
	return (false);
}

static bool is_raycast_edge(t_rayhit *rayhits, int x)
{
	bool		is_edge;
	t_rayhit	current;
	t_rayhit	left;
	t_rayhit	right;
	
	is_edge = false;
	current = rayhits[x];
	if (!current.hit)
		return (false);
	if (x > 0)
	{
		left = rayhits[x - 1];
		is_edge = find_neighbor_edge(&current, &left);	
	}
	if (x < MAX_WINDOW_WIDTH - 1)
	{
		right = rayhits[x + 1];
		if (!is_edge)
			is_edge = find_neighbor_edge(&current, &right);
	}
	return (is_edge);
}

static void paint_vertical_edge(int x, int y_start, int y_end, mlx_image_t *img)
{
	int	i;
	
	if (x < 0 || x >= (int)img->width)
		return ;

	// Clamp y coordinates to image bounds
	if (y_start < 0)
		y_start = 0;
	if (y_end >= (int)img->height)
		y_end = (int)img->height - 1;

	if (y_start >= y_end)
		return ;
	
	i = y_start;
	while (i <= y_end)
	{
		mlx_put_pixel(img, x, i, WHITE);
		i++;
	}
}

static void draw_vertical_outline(int x, t_rayhit rayhit, mlx_image_t *img)
{
	int		slice_height;
	int		draw_start;
	int		draw_end;
	
	if (!rayhit.hit)
		return ;
	
	slice_height =
		(int)(WORLDMAP_TILE_SIZE * MAX_WINDOW_HEIGHT / rayhit.distance);
	
	draw_start = -slice_height / 2 + MAX_WINDOW_HEIGHT / 2;
	if (draw_start < 0)
		draw_start = 0;
	draw_end = slice_height / 2 + MAX_WINDOW_HEIGHT / 2;
	if (draw_end >= MAX_WINDOW_HEIGHT)
		draw_end = MAX_WINDOW_HEIGHT - 1;
	paint_vertical_edge(x, draw_start, draw_end, img);
}

void add_wall_outlines(t_rayhit *rayhits, mlx_image_t *img)
{
	int i;
	
	i = 0;
	while (i < MAX_WINDOW_WIDTH)
	{
		if (is_raycast_edge(rayhits, i))
			draw_vertical_outline(i, rayhits[i], img);
		i++;
	}
}
