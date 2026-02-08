/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_neighbors_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 03:45:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 03:30:53 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	connect_neighbor_floors_bonus(t_floor *current,
		t_floor *neighbor, int is_up)
{
	if (is_up)
	{
		current->up = neighbor;
		if (!neighbor->down)
			neighbor->down = current;
	}
	else
	{
		current->down = neighbor;
		if (!neighbor->up)
			neighbor->up = current;
	}
}

int	attach_existing_neighbor_bonus(t_floor *current,
		t_floor *neighbor, int is_up, int index)
{
	neighbor->index = index;
	connect_neighbor_floors_bonus(current, neighbor, is_up);
	return (1);
}

void	push_floor_node_bonus(t_cub_data *data, t_floor *neighbor)
{
	neighbor->next = data->floors;
	data->floors = neighbor;
	data->floor_count++;
}

int	parse_neighbor_floor_bonus(t_floor *current,
		const char *neighbor_path, int is_up, t_cub_data *data)
{
	t_floor	*neighbor;
	int		index;
	char	*normalized;

	if (!neighbor_path || *neighbor_path == '\0')
		return (1);
	normalized = normalize_path_bonus(neighbor_path);
	if (!normalized)
		return (0);
	neighbor = find_floor_by_path_bonus(data->floors, normalized);
	index = current->index - 1;
	if (is_up)
		index = current->index + 1;
	free(normalized);
	if (neighbor)
		return (attach_existing_neighbor_bonus(current, neighbor, is_up,
				index));
	if (!parse_floor_file_bonus(neighbor_path, index, data, &neighbor))
		return (0);
	push_floor_node_bonus(data, neighbor);
	connect_neighbor_floors_bonus(current, neighbor, is_up);
	return (1);
}

int	parse_floor_neighbors_bonus(t_floor *floor, t_cub_data *data)
{
	if (!floor || floor->parsed_neighbors)
		return (1);
	floor->parsed_neighbors = 1;
	if (!parse_neighbor_floor_bonus(floor, floor->up_path, 1, data))
		return (0);
	if (!parse_neighbor_floor_bonus(floor, floor->down_path, 0, data))
		return (0);
	if (floor->up && !parse_floor_neighbors_bonus(floor->up, data))
		return (0);
	if (floor->down && !parse_floor_neighbors_bonus(floor->down, data))
		return (0);
	return (1);
}
