/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_manager_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/01/31 16:22:57 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "structs.h"

static char	*normalize_path_bonus(const char *path)
{
	char	*resolved;

	if (!path)
		return (NULL);
	resolved = realpath(path, NULL);
	if (!resolved)
		return (ft_strdup(path));
	return (resolved);
}

static int	is_elevator_char_bonus(char c)
{
	const char	*set = "!\"·$%&/()=?¿";
	int			i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static int	collect_floor_elevators_bonus(t_floor *floor)
{
	int		y;
	int		x;
	char	id;
	int		seen[256];

	ft_bzero(seen, sizeof(seen));
	floor->elevator_count = 0;
	y = 0;
	while (y < floor->map.height)
	{
		x = 0;
		while (floor->map.grid[y][x])
		{
			id = floor->map.grid[y][x];
			if (is_elevator_char_bonus(id))
			{
				if (seen[(unsigned char)id])
				{
					printf("Error: Duplicate elevator '%c' in floor %s\n",
						id, floor->path);
					return (0);
				}
				seen[(unsigned char)id] = 1;
				floor->elevator_ids[floor->elevator_count] = id;
				floor->elevator_x[floor->elevator_count] = x;
				floor->elevator_y[floor->elevator_count] = y;
				floor->elevator_count++;
			}
			x++;
		}
		y++;
	}
	return (1);
}

static int	register_floor_elevators_bonus(t_cub_data *data, t_floor *floor)
{
	int	i;
	int	slot;

	i = 0;
	while (i < floor->elevator_count)
	{
		slot = 0;
		while (slot < data->elevator_id_count)
		{
			if (data->elevator_ids[slot] == floor->elevator_ids[i])
				break ;
			slot++;
		}
		if (slot == data->elevator_id_count)
		{
			if (data->elevator_id_count >= 16)
			{
				printf("Error: Too many elevator identifiers\n");
				return (0);
			}
			data->elevator_ids[slot] = floor->elevator_ids[i];
			data->elevator_id_count++;
		}
		if (!data->elevator_floor_a[slot])
			data->elevator_floor_a[slot] = floor;
		else if (data->elevator_floor_a[slot] != floor
			&& !data->elevator_floor_b[slot])
			data->elevator_floor_b[slot] = floor;
		else if (data->elevator_floor_a[slot] != floor
			&& data->elevator_floor_b[slot]
			&& data->elevator_floor_b[slot] != floor)
		{
			printf("Error: Elevator '%c' appears in more than two floors\n",
				floor->elevator_ids[i]);
			return (0);
		}
		i++;
	}
	return (1);
}

static t_floor	*find_floor_by_path_bonus(t_floor *head, const char *path)
{
	t_floor	*current;

	current = head;
	while (current)
	{
		if (current->path && ft_strcmp(current->path, (char *)path) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

static void	free_texture_paths_bonus(t_textures *textures)
{
	if (!textures)
		return ;
	if (textures->north_path)
	{
		free(textures->north_path);
		textures->north_path = NULL;
	}
	if (textures->south_path)
	{
		free(textures->south_path);
		textures->south_path = NULL;
	}
	if (textures->west_path)
	{
		free(textures->west_path);
		textures->west_path = NULL;
	}
	if (textures->east_path)
	{
		free(textures->east_path);
		textures->east_path = NULL;
	}
}

static void	free_floor_node_bonus(t_floor *floor)
{
	if (!floor)
		return ;
	if (floor->map.grid)
		free_map(&(floor->map));
	if (floor->path)
		free(floor->path);
	if (floor->up_path)
		free(floor->up_path);
	if (floor->down_path)
		free(floor->down_path);
	free_texture_paths_bonus(&floor->textures);
	free_textures(&floor->textures);
	free(floor);
}

static int	create_floor_from_data_bonus(char *canonical_path, t_cub_data *src,
		t_floor **out, int index)
{
	if (!canonical_path)
		return (0);
	*out = malloc(sizeof(t_floor));
	if (!*out)
	{
		free(canonical_path);
		return (0);
	}
	(*out)->path = canonical_path;
	(*out)->map = src->map;
	(*out)->up_path = src->up_path;
	(*out)->down_path = src->down_path;
	(*out)->textures = src->textures;
	(*out)->floor_color = src->floor_color;
	(*out)->ceiling_color = src->ceiling_color;
	(*out)->up = NULL;
	(*out)->down = NULL;
	(*out)->next = NULL;
	(*out)->index = index;
	(*out)->parsed_neighbors = 0;
	(*out)->has_player = (src->player.x >= 0.0f && src->player.y >= 0.0f);
	(*out)->player = src->player;
	(*out)->textures_loaded = false;
	ft_bzero(&src->map, sizeof(t_map));
	ft_bzero(&src->textures, sizeof(t_textures));
	ft_bzero(&src->floor_color, sizeof(t_color));
	ft_bzero(&src->ceiling_color, sizeof(t_color));
	src->up_path = NULL;
	src->down_path = NULL;
	if (!collect_floor_elevators_bonus(*out))
	{
		free_floor_node_bonus(*out);
		*out = NULL;
		return (0);
	}
	return (1);
}

static int	parse_floor_file_bonus(const char *path, int index,
		t_cub_data *global, t_floor **out_floor, int *player_count)
{
	t_cub_data	tmp;
	char		**lines;
	int			line_count;
	char		*canonical;

	if (!validate_file_extension(path))
	{
		printf("Error: Invalid floor file extension for %s\n", path);
		return (0);
	}
	canonical = normalize_path_bonus(path);
	if (!canonical)
		return (0);
	init_cub_data_parser_only(&tmp);
	lines = read_file_lines(canonical, &line_count);
	if (!lines)
	{
		printf("Error: Could not read floor file %s\n", path);
		free(canonical);
		return (0);
	}
	if (!process_file_data_bonus(lines, line_count, &tmp))
	{
		free_lines(lines, line_count);
		free(canonical);
		free_cub_data(&tmp);
		return (0);
	}
	free_lines(lines, line_count);
	if (!create_floor_from_data_bonus(canonical, &tmp, out_floor, index))
	{
		free_cub_data(&tmp);
		return (0);
	}
	free_cub_data(&tmp);
	if (!register_floor_elevators_bonus(global, *out_floor))
	{
		free_floor_node_bonus(*out_floor);
		*out_floor = NULL;
		return (0);
	}
	if ((*out_floor)->has_player)
	{
		if (*player_count)
		{
			printf("Error: Multiple player positions across floors\n");
			if (global->player_floor_path)
				printf("  - First player found in: %s\n",
					global->player_floor_path);
			printf("  - Another player found in: %s\n", canonical);
			free_floor_node_bonus(*out_floor);
			*out_floor = NULL;
			return (0);
		}
		*player_count = 1;
		global->player = (*out_floor)->player;
		global->player_floor_index = index;
		global->current_floor = *out_floor;
		global->map = (*out_floor)->map;
		if (global->player_floor_path)
			free(global->player_floor_path);
		global->player_floor_path = ft_strdup(path);
	}
	return (1);
}

static int	parse_neighbor_floor_bonus(t_floor *current,
	const char *neighbor_path, int is_up, t_cub_data *data, int *player_count)
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
	index = current->index + (is_up ? 1 : -1);
	if (neighbor)
	{
		free(normalized);
		neighbor->index = index;
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
		return (1);
	}
	if (!parse_floor_file_bonus(neighbor_path, index, data, &neighbor,
			player_count))
	{
		free(normalized);
		return (0);
	}
	neighbor->next = data->floors;
	data->floors = neighbor;
	data->floor_count++;
	free(normalized);
	if (is_up)
	{
		current->up = neighbor;
		neighbor->down = current;
	}
	else
	{
		current->down = neighbor;
		neighbor->up = current;
	}
	return (1);
}

static int	parse_floor_neighbors_bonus(t_floor *floor, t_cub_data *data,
		int *player_count)
{
	if (!floor || floor->parsed_neighbors)
		return (1);
	floor->parsed_neighbors = 1;
	if (!parse_neighbor_floor_bonus(floor, floor->up_path, 1, data,
			player_count))
		return (0);
	if (!parse_neighbor_floor_bonus(floor, floor->down_path, 0, data,
			player_count))
		return (0);
	if (floor->up && !parse_floor_neighbors_bonus(floor->up, data,
		player_count))
		return (0);
	if (floor->down
		&& !parse_floor_neighbors_bonus(floor->down, data, player_count))
		return (0);
	return (1);
}

static int	validate_global_elevators_bonus(t_cub_data *data)
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

int	build_floor_graph_bonus(const char *path, t_cub_data *data)
{
	int		player_count;
	t_floor	*root;
	char	*canonical_root;

	player_count = 0;
	canonical_root = normalize_path_bonus(path);
	if (!canonical_root)
		return (0);
	if (!create_floor_from_data_bonus(canonical_root, data, &root, 0))
		return (0);
	data->floors = root;
	data->current_floor = root;
	data->map = root->map;
	data->floor_count = 1;
	if (!register_floor_elevators_bonus(data, root))
		return (0);
	if (root->has_player)
	{
		player_count = 1;
		data->player_floor_index = 0;
		if (data->player_floor_path)
			free(data->player_floor_path);
		data->player_floor_path = ft_strdup(path);
	}
	if (!parse_floor_neighbors_bonus(root, data, &player_count))
		return (0);
	if (player_count != 1)
	{
		printf("Error: Expected exactly one player across all floors\n");
		return (0);
	}
	if (!validate_global_elevators_bonus(data))
		return (0);
	data->textures = data->current_floor->textures;
	data->floor_color = data->current_floor->floor_color;
	data->ceiling_color = data->current_floor->ceiling_color;
	return (1);
}
