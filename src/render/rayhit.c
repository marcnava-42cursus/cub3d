/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 09:00:55 by ivmirand          #+#    #+#             */
/*   Updated: 2025/12/26 14:01:24 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static void	set_ray_direction(vertex_t *direction, float angle)
{
	direction->x = cosf(angle);
	direction->y = sinf(angle);
	if (fabsf(direction->x) < 1e-8f)
	{
		if (direction->x < 0)
			direction->x = -1e-8f;
		else
			direction->x = 1e-8f;
	}
	if (fabsf(direction->y) < 1e-8f)
	{
		if (direction->y < 0)
			direction->y = -1e-8f;
		else
			direction->y = 1e-8f;
	}
}

static void	set_ray_steps(t_rayhit *rayhit, vertex_t *direction, vertex_t start)
{
	rayhit->hit = true;
	rayhit->cell_x = (int)floorf(start.x / WORLDMAP_TILE_SIZE);
	rayhit->cell_y = (int)floorf(start.y / WORLDMAP_TILE_SIZE);
	if (direction->x > 0.0f)
		rayhit->step_x = 1;
	else
		rayhit->step_x = -1;
	if (direction->y > 0.0f)
		rayhit->step_y = 1;
	else
		rayhit->step_y = -1;
}

void	init_rayhit(t_rayhit *rayhit, vertex_t start, vertex_t *direction,
		float angle)
{
	rayhit->hit = false;
	rayhit->cell_x = -1;
	rayhit->cell_y = -1;
	rayhit->step_x = -1;
	rayhit->step_y = -1;
	rayhit->side = -1;
	rayhit->position = start;
	rayhit->distance = 0.0f;
	rayhit->face = NORTH;
	set_ray_direction(direction, angle);
	set_ray_steps(rayhit, direction, start);
}
