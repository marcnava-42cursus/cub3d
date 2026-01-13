/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 12:06:00 by ivmirand          #+#    #+#             */
/*   Updated: 2026/01/13 17:52:10 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static t_rayhit	*set_successful_rayhit(t_rayhit *rayhit, vertex_t *start,
		vertex_t *direction, float t)
{
	if (rayhit->side == 0)
	{
		if (rayhit->step[X] > 0)
			rayhit->face = WEST;
		else
			rayhit->face = EAST;
	}
	else
	{
		if (rayhit->step[Y] > 0)
			rayhit->face = SOUTH;
		else
			rayhit->face = NORTH;
	}
	rayhit->hit = true;
	rayhit->position.x = start->x + direction->x * t;
	rayhit->position.y = start->y + direction->y * t;
	rayhit->distance = t;
	return (rayhit);
}

static float	get_t(t_rayhit *rayhit, vertex_t *t_max, vertex_t direction)
{
	float	t;

	if (t_max->x < t_max->y)
	{
		rayhit->cell[X] += rayhit->step[X];
		t = t_max->x;
		t_max->x += fabsf(WORLDMAP_TILE_SIZE / direction.x);
		rayhit->side = 0;
	}
	else
	{
		rayhit->cell[Y] += rayhit->step[Y];
		t = t_max->y;
		t_max->y += fabsf(WORLDMAP_TILE_SIZE / direction.y);
		rayhit->side = 1;
	}
	return (t);
}

static void	init_t_max(t_rayhit *rayhit, vertex_t *t_max, vertex_t start,
		vertex_t direction)
{
	vertex_t	next_grid;

	if (rayhit->step[X] > 0)
		next_grid.x = (rayhit->cell[X] + 1) * WORLDMAP_TILE_SIZE;
	else
		next_grid.x = rayhit->cell[X] * WORLDMAP_TILE_SIZE;
	if (rayhit->step[Y] > 0)
		next_grid.y = (rayhit->cell[Y] + 1) * WORLDMAP_TILE_SIZE;
	else
		next_grid.y = rayhit->cell[Y] * WORLDMAP_TILE_SIZE;
	t_max->x = (next_grid.x - start.x) / direction.x;
	t_max->y = (next_grid.y - start.y) / direction.y;
}

static bool	read_cell(const t_map *map, int cell[2])
{
	const char	*row;
	char		cell_char;

	if (!map || !map->grid || map->height <= 0 || cell[Y] < 0
		|| cell[Y] >= map->height)
		return (false);
	row = map->grid[cell[Y]];
	if (!row)
		return (false);
	if (cell[X] < 0 || cell[X] >= (int)ft_strlen(row))
		return (false);
	cell_char = row[cell[X]];
	if (cell_char == '1' || cell_char == '2' || cell_char == ' '
		|| cell_char == '\0')
		return (false);
	return (true);
}

t_rayhit	raycast_world(const t_map *map, vertex_t start, float angle,
		float max_distance)
{
	t_rayhit	rayhit;
	vertex_t	direction;
	vertex_t	t_max;
	float		t;

	t = 0.0f;
	init_rayhit(&rayhit, start, &direction, angle);
	if (!read_cell(map, rayhit.cell))
		return (rayhit);
	init_t_max(&rayhit, &t_max, start, direction);
	while (t <= max_distance)
	{
		t = get_t(&rayhit, &t_max, direction);
		if (rayhit.cell[Y] < 0 || rayhit.cell[Y] >= map->height
			|| !map->grid[rayhit.cell[Y]] || rayhit.cell[X] < 0
			|| rayhit.cell[X] >= (int)ft_strlen(map->grid[rayhit.cell[Y]])
			|| !read_cell(map, rayhit.cell))
			return (*set_successful_rayhit(&rayhit, &start, &direction, t));
	}
	rayhit.hit = false;
	rayhit.distance = fminf(t, max_distance);
	rayhit.position.x = start.x + direction.x * rayhit.distance;
	rayhit.position.y = start.y + direction.y * rayhit.distance;
	return (rayhit);
}
