/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_graph_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 03:45:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 03:30:53 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	validate_global_elevators_bonus(t_cub_data *data)
{
	int	i;

	i = 0;
	while (i < data->elevator_id_count)
	{
		if (!data->elevator_floor_a[i] || !data->elevator_floor_b[i])
		{
			printf("Error: Elevator '%c' is not paired between two floors\n",
				data->elevator_ids[i]);
			return (0);
		}
		i++;
	}
	return (1);
}

int	setup_root_floor_bonus(const char *path, t_cub_data *data,
		t_floor **root)
{
	char	*canonical_root;

	canonical_root = normalize_path_bonus(path);
	if (!canonical_root)
		return (0);
	if (!create_floor_from_data_bonus(canonical_root, data, root, 0))
		return (0);
	data->floors = *root;
	data->current_floor = *root;
	data->map = (*root)->map;
	data->floor_count = 1;
	if (!register_floor_elevators_bonus(data, *root))
		return (0);
	if ((*root)->has_player)
		assign_global_player_floor_bonus(data, *root, 0, path);
	return (1);
}

int	validate_player_presence_bonus(t_cub_data *data)
{
	if (!data->player_floor_path)
	{
		printf("Error: Expected exactly one player across all floors\n");
		return (0);
	}
	return (1);
}

void	sync_current_floor_visuals_bonus(t_cub_data *data)
{
	data->textures = data->current_floor->textures;
	data->floor_color = data->current_floor->floor_color;
	data->ceiling_color = data->current_floor->ceiling_color;
}

int	build_floor_graph_bonus(const char *path, t_cub_data *data)
{
	t_floor	*root;

	if (!setup_root_floor_bonus(path, data, &root))
		return (0);
	if (!parse_floor_neighbors_bonus(root, data))
		return (0);
	if (!validate_player_presence_bonus(data))
		return (0);
	if (!validate_global_elevators_bonus(data))
		return (0);
	sync_current_floor_visuals_bonus(data);
	return (1);
}
