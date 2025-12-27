/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 12:06:00 by ivmirand          #+#    #+#             */
/*   Updated: 2025/12/26 13:59:51 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static t_rayhit	*set_successful_rayhit(t_rayhit *rayhit, vertex_t *start,
		vertex_t *direction, float t)
{
	if (rayhit->side == 0)
	{
		if (rayhit->step_x > 0)
			rayhit->face = WEST;
		else
			rayhit->face = EAST;
	}
	else
	{
		if (rayhit->step_y > 0)
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
		rayhit->cell_x += rayhit->step_x;
		t = t_max->x;
		t_max->x += fabsf(WORLDMAP_TILE_SIZE / direction.x);
		rayhit->side = 0;
	}
	else
	{
		rayhit->cell_y += rayhit->step_y;
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

	if (rayhit->step_x > 0)
		next_grid.x = (rayhit->cell_x + 1) * WORLDMAP_TILE_SIZE;
	else
		next_grid.x = rayhit->cell_x * WORLDMAP_TILE_SIZE;
	if (rayhit->step_y > 0)
		next_grid.y = (rayhit->cell_y + 1) * WORLDMAP_TILE_SIZE;
	else
		next_grid.y = rayhit->cell_y * WORLDMAP_TILE_SIZE;
	t_max->x = (next_grid.x - start.x) / direction.x;
	t_max->y = (next_grid.y - start.y) / direction.y;
}

static bool	read_cell(const t_map *map, int x, int y)
{
	const char	*row;
	char		cell;

	if (!map || !map->grid || map->height <= 0 || y < 0 || y >= map->height)
		return (false);
	row = map->grid[y];
	if (!row)
		return (false);
	if (x < 0 || x >= (int)ft_strlen(row))
		return (false);
	cell = row[x];
	if (cell == '1' || cell == '2' || cell == ' ' || cell == '\0')
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

	init_rayhit(&rayhit, start, &direction, angle);
	if (!read_cell(map, rayhit.cell_x, rayhit.cell_y))
		return (rayhit);
	init_t_max(&rayhit, &t_max, start, direction);
	while (t <= max_distance)
	{
		t = get_t(&rayhit, &t_max, direction);
		if (rayhit.cell_y < 0 || rayhit.cell_y >= map->height
			|| !map->grid[rayhit.cell_y] || rayhit.cell_x < 0
			|| rayhit.cell_x >= (int)ft_strlen(map->grid[rayhit.cell_y])
			|| !read_cell(map, rayhit.cell_x, rayhit.cell_y))
			return (*set_successful_rayhit(&rayhit, &start, &direction, t));
	}
	rayhit.hit = false;
	rayhit.distance = fminf(t, max_distance);
	rayhit.position.x = start.x + direction.x * rayhit.distance;
	rayhit.position.y = start.y + direction.y * rayhit.distance;
	return (rayhit);
}
