/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivmirand <ivmirand@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 12:06:00 by ivmirand          #+#    #+#             */
/*   Updated: 2025/09/19 10:57:42 by ivmirand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static bool	read_cell(const t_map *map, int x, int y, char *out)
{
	const char	*row;
	int			len;

	if (!map || !map->grid || y < 0 || y >= map->height)
		return (false);
	row = map->grid[y];
	if (!row)
		return (false);
	len = (int)strlen(row);
	if (x < 0 || x >= len)
		return (false);
	*out = row[x];
	return (true);
}

t_rayhit	raycast_world(const t_map *map, vertex_t start, float angle,
		float max_distance)
{
	t_rayhit	rayhit;
	vertex_t	direction;
	vertex_t	delta_distance;
	vertex_t	next_grid;
	vertex_t 	t_max;
	int			map_x;
	int			map_y;
	int			step_x;
	int			step_y;
	int			row_len;
	float		t;
	char		cell;
	
	rayhit.hit = false;
	rayhit.cell_x = -1;
	rayhit.cell_y = -1;
	rayhit.side = -1;
	rayhit.position = start;
	rayhit.distance = 0.0f;

	if (!map || !map->grid || map->height <= 0)
		return (rayhit);

	direction.x = cosf(angle);
	direction.y = -sinf(angle);

	if (fabsf(direction.x) < 1e-8f)
	{
		if (direction.x < 0)
			direction.x = -1e-8f;
		else
			direction.x = 1e-8f;
	}	
	if (fabsf(direction.y) < 1e-8f)
	{
		if (direction.y < 0)
			direction.y = -1e-8f;
		else
			direction.y = 1e-8f;
	}	

	map_x = (int)floorf(start.x / WORLDMAP_TILE_SIZE);
	map_y = (int)floorf(start.y / WORLDMAP_TILE_SIZE);

	if (!read_cell(map, map_x, map_y, &cell)
		|| cell == '1' || cell == ' ' || cell == '\0')
	{
		rayhit.hit = true;
		rayhit.cell_x = map_x;
		rayhit.cell_y = map_y;
		return (rayhit);
	}

	if (direction.x > 0.0f)
		step_x = 1;
	else
		step_x = -1;	
	if (direction.y > 0.0f)
		step_y = 1;
	else
		step_y = -1;	

	if (step_x > 0)
		next_grid.x = (map_x + 1) * WORLDMAP_TILE_SIZE;
	else
		next_grid.x = map_x * WORLDMAP_TILE_SIZE;
	if (step_y > 0)
		next_grid.y = (map_y + 1) * WORLDMAP_TILE_SIZE;
	else
		next_grid.y = map_y * WORLDMAP_TILE_SIZE;

	t_max.x = (next_grid.x - start.x) / direction.x;
	t_max.y = (next_grid.y - start.y) / direction.y;
	delta_distance.x = fabsf(WORLDMAP_TILE_SIZE / direction.x);
	delta_distance.y = fabsf(WORLDMAP_TILE_SIZE / direction.y);

	t = 0.0f;
	while (t <= max_distance)
	{
		if (t_max.x < t_max.y)
		{
			map_x += step_x;
			t = t_max.x;
			t_max.x += delta_distance.x;
			rayhit.side = 0;
		}
		else
		{
			map_y += step_y;
			t = t_max.y;
			t_max.y += delta_distance.y;
			rayhit.side = 1;
		}
		if (map_y < 0 || map_y >= map->height || !map->grid[map_y])
		{
			rayhit.hit = true;
			rayhit.cell_x = map_x;
			rayhit.cell_y = map_y;
			rayhit.position.x = start.x + direction.x * t;
			rayhit.position.y = start.y + direction.y * t;
			rayhit.distance = t;
			return (rayhit);
		}
		row_len = (int)strlen(map->grid[map_y]);
		if (map_x < 0 || map_x >= row_len)
		{
			rayhit.hit = true;
			rayhit.cell_x = map_x;
			rayhit.cell_y = map_y;
			rayhit.position.x = start.x + direction.x * t;
			rayhit.position.y = start.y + direction.y * t;
			rayhit.distance = t;
			return (rayhit);
		}
		cell = map->grid[map_y][map_x];
		if (cell == '1' || cell == ' ' || cell == '\0')
		{
			rayhit.hit = true;
			rayhit.cell_x = map_x;
			rayhit.cell_y = map_y;
			rayhit.position.x = start.x + direction.x * t;
			rayhit.position.y = start.y + direction.y * t;
			rayhit.distance = t;
			return (rayhit);
		}
	}
	// if no hit
	rayhit.hit = false;
	rayhit.distance = fminf(t, max_distance);
	rayhit.position.x = start.x + direction.x * rayhit.distance;
	rayhit.position.y = start.y + direction.y * rayhit.distance;
	rayhit.cell_x = map_x;
	rayhit.cell_y = map_y;
	return (rayhit);
}
