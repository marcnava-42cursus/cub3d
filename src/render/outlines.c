/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   outlines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 13:35:49 by ivmirand          #+#    #+#             */
/*   Updated: 2025/09/19 18:51:57 by ivmirand         ###   ########.fr       */
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

//draw[0] is start and draw[1] is end
static void draw_vertical_outline(int x, t_rayhit rayhit, mlx_image_t *img)
{
	int		slice_height;
	int		draw[2];
	
	if (!rayhit.hit)
		return ;
	
	slice_height =
		(int)(WORLDMAP_TILE_SIZE * MAX_WINDOW_HEIGHT / rayhit.distance);
	
	draw[0] = -slice_height / 2 + MAX_WINDOW_HEIGHT / 2;
	if (draw[0] < 0)
		draw[0] = 0;
	draw[1] = slice_height / 2 + MAX_WINDOW_HEIGHT / 2;
	if (draw[1] >= MAX_WINDOW_HEIGHT)
		draw[1] = MAX_WINDOW_HEIGHT - 1;
	paint_vertical_line(x, draw, img, WHITE);
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
