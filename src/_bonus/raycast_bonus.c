/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/11/13 20:03:33 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static void set_ray_direction_and_coords(vertex_t *direction, float angle, vertex_t start, int ray_coords[4])
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
	ray_coords[0] = (int)floorf(start.x / WORLDMAP_TILE_SIZE);
	ray_coords[1] = (int)floorf(start.y / WORLDMAP_TILE_SIZE);
	if (direction->x > 0.0f)
		ray_coords[2] = 1;
	else
		ray_coords[2] = -1;
	if (direction->y > 0.0f)
		ray_coords[3] = 1;
	else
		ray_coords[3] = -1;
}

static bool	read_cell_bonus(const t_map *map, int x, int y)
{
	const char	*row;
	char		cell;
	int			len;

	if (!map || !map->grid || y < 0 || y >= map->height)
		return (false);
	row = map->grid[y];
	if (!row)
		return (false);
	len = (int)strlen(row);
	if (x < 0 || x >= len)
		return (false);
	cell = row[x];
	if (cell == '1' || cell == '2' || cell == ' ' || cell == '\0')
		return (false);
	// Block ray for both uppercase AND lowercase special chars (for rendering)
	if (ft_isalpha(cell) && cell != 'N' && cell != 'S' && cell != 'E' && cell != 'W')
		return (false);
	return (true);
}

static void	set_rayhit_face(t_rayhit *rayhit, int ray_coords[4])
{
	if (rayhit->side == 0)
	{
		if (ray_coords[2] > 0)
			rayhit->face = WEST;
		else
			rayhit->face = EAST;
	}
	else
	{
		if (ray_coords[3] > 0)
			rayhit->face = SOUTH;
		else
			rayhit->face = NORTH;
	}
}

t_rayhit	raycast_world_bonus(const t_map *map, vertex_t start, float angle,
		float max_distance)
{
	t_rayhit	rayhit;
	vertex_t	direction;
	vertex_t	delta_distance;
	vertex_t	next_grid;
	vertex_t 	t_max;
	int			ray_coords[4];
	int			row_len;
	float		t;

	rayhit.hit = false;
	rayhit.cell_x = -1;
	rayhit.cell_y = -1;
	rayhit.side = -1;
	rayhit.position = start;
	rayhit.distance = 0.0f;
	rayhit.face = NORTH;

	if (!map || !map->grid || map->height <= 0)
		return (rayhit);
	set_ray_direction_and_coords(&direction, angle, start, ray_coords);
	if (!read_cell_bonus(map, ray_coords[0], ray_coords[1]))
	{
		rayhit.hit = true;
		rayhit.cell_x = ray_coords[0];
		rayhit.cell_y = ray_coords[1];
		return (rayhit);
	}

	if (ray_coords[2] > 0)
		next_grid.x = (ray_coords[0] + 1) * WORLDMAP_TILE_SIZE;
	else
		next_grid.x = ray_coords[0] * WORLDMAP_TILE_SIZE;
	if (ray_coords[3] > 0)
		next_grid.y = (ray_coords[1] + 1) * WORLDMAP_TILE_SIZE;
	else
		next_grid.y = ray_coords[1] * WORLDMAP_TILE_SIZE;

	t_max.x = (next_grid.x - start.x) / direction.x;
	t_max.y = (next_grid.y - start.y) / direction.y;
	delta_distance.x = fabsf(WORLDMAP_TILE_SIZE / direction.x);
	delta_distance.y = fabsf(WORLDMAP_TILE_SIZE / direction.y);

	t = 0.0f;
	while (t <= max_distance)
	{
		if (t_max.x < t_max.y)
		{
			ray_coords[0] += ray_coords[2];
			t = t_max.x;
			t_max.x += delta_distance.x;
			rayhit.side = 0;
		}
		else
		{
			ray_coords[1] += ray_coords[3];
			t = t_max.y;
			t_max.y += delta_distance.y;
			rayhit.side = 1;
		}
		if (ray_coords[1] < 0 || ray_coords[1] >= map->height
			|| !map->grid[ray_coords[1]])
		{
			set_rayhit_face(&rayhit, ray_coords);
			rayhit.hit = true;
			rayhit.cell_x = ray_coords[0];
			rayhit.cell_y = ray_coords[1];
			rayhit.position.x = start.x + direction.x * t;
			rayhit.position.y = start.y + direction.y * t;
			rayhit.distance = t;
			return (rayhit);
		}
		row_len = (int)strlen(map->grid[ray_coords[1]]);
		if (ray_coords[0] < 0 || ray_coords[0] >= row_len)
		{
			set_rayhit_face(&rayhit, ray_coords);
			rayhit.hit = true;
			rayhit.cell_x = ray_coords[0];
			rayhit.cell_y = ray_coords[1];
			rayhit.position.x = start.x + direction.x * t;
			rayhit.position.y = start.y + direction.y * t;
			rayhit.distance = t;
			return (rayhit);
		}
		if (!read_cell_bonus(map, ray_coords[0], ray_coords[1]))
		{
			set_rayhit_face(&rayhit, ray_coords);
			rayhit.hit = true;
			rayhit.cell_x = ray_coords[0];
			rayhit.cell_y = ray_coords[1];
			rayhit.position.x = start.x + direction.x * t;
			rayhit.position.y = start.y + direction.y * t;
			rayhit.distance = t;
			return (rayhit);
		}
	}
	return (rayhit);
}
