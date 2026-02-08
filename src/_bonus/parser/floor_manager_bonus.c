/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_manager_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/07 19:12:30 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "structs.h"

static int	register_floor_elevator_advanced(t_floor *floor, int seen[256],
				char id, int cell[2]);

static char	*alloc_normalized_path_advanced(size_t size, bool abs_path)
{
	char	*normalized;

	normalized = malloc(size);
	if (!normalized)
		return (NULL);
	normalized[0] = '\0';
	if (abs_path)
		ft_strlcpy(normalized, "/", size);
	return (normalized);
}

static void	ensure_non_empty_path_advanced(char *normalized, bool abs_path,
				size_t size)
{
	if (normalized[0] != '\0')
		return ;
	if (abs_path)
		ft_strlcpy(normalized, "/", size);
	else
		ft_strlcpy(normalized, ".", size);
}

static void	free_path_tokens(char **tokens, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

static int	is_empty_or_dot_token_advanced(const char *token)
{
	if (token[0] == '\0')
		return (1);
	return (token[0] == '.' && token[1] == '\0');
}

static int	is_parent_token_advanced(const char *token)
{
	return (token[0] == '.' && token[1] == '.' && token[2] == '\0');
}

static void	handle_parent_token_advanced(char **tokens, int *top, int i,
		bool abs_path)
{
	if (*top > 0 && ft_strcmp(tokens[*top - 1], "..") != 0)
	{
		free(tokens[--(*top)]);
		free(tokens[i]);
	}
	else if (!abs_path)
		tokens[(*top)++] = tokens[i];
	else
		free(tokens[i]);
}

static int	collect_normalized_tokens_advanced(char **tokens, bool abs_path)
{
	int	i;
	int	top;

	top = 0;
	i = 0;
	while (tokens[i])
	{
		if (is_empty_or_dot_token_advanced(tokens[i]))
			free(tokens[i]);
		else if (is_parent_token_advanced(tokens[i]))
			handle_parent_token_advanced(tokens, &top, i, abs_path);
		else
			tokens[top++] = tokens[i];
		i++;
	}
	return (top);
}

static void	append_tokens_to_path_advanced(char *normalized, size_t size,
		char **tokens, int top)
{
	int	i;

	i = 0;
	while (i < top)
	{
		if (normalized[0] != '\0'
			&& normalized[ft_strlen(normalized) - 1] != '/')
			ft_strlcat(normalized, "/", size);
		ft_strlcat(normalized, tokens[i], size);
		i++;
	}
}

static char	*normalize_path_advanced(const char *path)
{
	char	**tokens;
	char	*normalized;
	size_t	size;
	int		top;
	bool	abs_path;

	if (!path)
		return (NULL);
	tokens = ft_split(path, '/');
	if (!tokens)
		return (NULL);
	abs_path = (path[0] == '/');
	top = collect_normalized_tokens_advanced(tokens, abs_path);
	size = ft_strlen(path) + 3;
	normalized = alloc_normalized_path_advanced(size, abs_path);
	if (!normalized)
		return (free_path_tokens(tokens, top), NULL);
	append_tokens_to_path_advanced(normalized, size, tokens, top);
	ensure_non_empty_path_advanced(normalized, abs_path, size);
	return (free_path_tokens(tokens, top), normalized);
}

static int	is_elevator_char_advanced(char c)
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

static int	collect_floor_elevators_advanced(t_floor *floor)
{
	int		y;
	int		x;
	char	id;
	int		seen[256];
	int		cell[2];

	ft_bzero(seen, sizeof(seen));
	floor->elevator_count = 0;
	y = 0;
	while (y < floor->map.height)
	{
		x = 0;
		while (floor->map.grid[y][x])
		{
			id = floor->map.grid[y][x];
			cell[0] = x;
			cell[1] = y;
			if (is_elevator_char_advanced(id)
				&& !register_floor_elevator_advanced(floor, seen, id, cell))
				return (0);
			x++;
		}
		y++;
	}
	return (1);
}

static int	register_floor_elevator_advanced(t_floor *floor, int seen[256],
		char id, int cell[2])
{
	if (seen[(unsigned char)id])
	{
		printf("Error: Duplicate elevator '%c' in floor %s\n", id, floor->path);
		return (0);
	}
	seen[(unsigned char)id] = 1;
	floor->elevator_ids[floor->elevator_count] = id;
	floor->elevator_x[floor->elevator_count] = cell[0];
	floor->elevator_y[floor->elevator_count] = cell[1];
	floor->elevator_count++;
	return (1);
}

static int	find_or_create_elevator_slot_advanced(t_cub_data *data, char id)
{
	int	slot;

	slot = 0;
	while (slot < data->elevator_id_count)
	{
		if (data->elevator_ids[slot] == id)
			return (slot);
		slot++;
	}
	if (data->elevator_id_count >= 16)
	{
		printf("Error: Too many elevator identifiers\n");
		return (-1);
	}
	data->elevator_ids[slot] = id;
	data->elevator_id_count++;
	return (slot);
}

static int	register_floor_in_slot_advanced(t_cub_data *data, t_floor *floor,
			int slot, char id)
{
	if (!data->elevator_floor_a[slot])
		data->elevator_floor_a[slot] = floor;
	else if (data->elevator_floor_a[slot] != floor
		&& !data->elevator_floor_b[slot])
		data->elevator_floor_b[slot] = floor;
	else if (data->elevator_floor_a[slot] != floor
		&& data->elevator_floor_b[slot]
		&& data->elevator_floor_b[slot] != floor)
	{
		printf("Error: Elevator '%c' appears in more than two floors\n", id);
		return (0);
	}
	return (1);
}

static int	register_floor_elevators_advanced(t_cub_data *data, t_floor *floor)
{
	int	i;
	int	slot;

	i = 0;
	while (i < floor->elevator_count)
	{
		slot = find_or_create_elevator_slot_advanced(data,
				floor->elevator_ids[i]);
		if (slot < 0)
			return (0);
		if (!register_floor_in_slot_advanced(data, floor, slot,
				floor->elevator_ids[i]))
			return (0);
		i++;
	}
	return (1);
}

static t_floor	*find_floor_by_path_advanced(t_floor *head, const char *path)
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

static void	free_texture_paths_advanced(t_textures *textures)
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

static void	free_floor_node_advanced(t_floor *floor)
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
	free_texture_paths_advanced(&floor->textures);
	free_textures(&floor->textures);
	free(floor);
}

static void	init_floor_node_fields_advanced(t_floor *floor,
		char *canonical_path, t_cub_data *src, int index)
{
	floor->path = canonical_path;
	floor->map = src->map;
	floor->up_path = src->up_path;
	floor->down_path = src->down_path;
	floor->textures = src->textures;
	floor->floor_color = src->floor_color;
	floor->ceiling_color = src->ceiling_color;
	floor->up = NULL;
	floor->down = NULL;
	floor->next = NULL;
	floor->index = index;
	floor->parsed_neighbors = 0;
	floor->has_player = (src->player.x >= 0.0f && src->player.y >= 0.0f);
	floor->player = src->player;
	floor->textures_loaded = false;
}

static void	reset_source_floor_data_advanced(t_cub_data *src)
{
	ft_bzero(&src->map, sizeof(t_map));
	ft_bzero(&src->textures, sizeof(t_textures));
	ft_bzero(&src->floor_color, sizeof(t_color));
	ft_bzero(&src->ceiling_color, sizeof(t_color));
	src->up_path = NULL;
	src->down_path = NULL;
}

static int	create_floor_from_data_advanced(char *canonical_path,
		t_cub_data *src, t_floor **out, int index)
{
	if (!canonical_path)
		return (0);
	*out = malloc(sizeof(t_floor));
	if (!*out)
	{
		free(canonical_path);
		return (0);
	}
	init_floor_node_fields_advanced(*out, canonical_path, src, index);
	reset_source_floor_data_advanced(src);
	if (!collect_floor_elevators_advanced(*out))
	{
		free_floor_node_advanced(*out);
		*out = NULL;
		return (0);
	}
	return (1);
}

static int	prepare_tmp_floor_parse_advanced(const char *path, char *canonical,
		t_cub_data *tmp)
{
	char	**lines;
	int		line_count;

	init_cub_data_parser_only(tmp);
	lines = read_file_lines(canonical, &line_count);
	if (!lines)
	{
		printf("Error: Could not read floor file %s\n", path);
		return (0);
	}
	if (!process_file_data_advanced(lines, line_count, tmp))
		return (free_lines(lines, line_count), free_cub_data(tmp), 0);
	free_lines(lines, line_count);
	return (1);
}

static void	assign_global_player_floor_advanced(t_cub_data *global,
		t_floor *floor, int index, const char *path)
{
	global->player = floor->player;
	global->player_floor_index = index;
	global->current_floor = floor;
	global->map = floor->map;
	if (global->player_floor_path)
		free(global->player_floor_path);
	global->player_floor_path = ft_strdup(path);
}

static int	handle_floor_player_advanced(t_cub_data *global,
		t_floor **out_floor, int index)
{
	if (!(*out_floor)->has_player)
		return (1);
	if (global->player_floor_path)
	{
		printf("Error: Multiple player positions across floors\n");
		if (global->player_floor_path)
			printf("  - First player found in: %s\n",
				global->player_floor_path);
		printf("  - Another player found in: %s\n", (*out_floor)->path);
		free_floor_node_advanced(*out_floor);
		*out_floor = NULL;
		return (0);
	}
	assign_global_player_floor_advanced(global, *out_floor, index,
		(*out_floor)->path);
	return (1);
}

static int	parse_floor_file_advanced(const char *path, int index,
			t_cub_data *global, t_floor **out_floor)
{
	t_cub_data	tmp;
	char		*canonical;

	if (!validate_file_extension(path))
	{
		printf("Error: Invalid floor file extension for %s\n", path);
		return (0);
	}
	canonical = normalize_path_advanced(path);
	if (!canonical)
		return (0);
	if (!prepare_tmp_floor_parse_advanced(path, canonical, &tmp))
		return (free(canonical), 0);
	if (!create_floor_from_data_advanced(canonical, &tmp, out_floor, index))
		return (free_cub_data(&tmp), 0);
	free_cub_data(&tmp);
	if (!register_floor_elevators_advanced(global, *out_floor))
	{
		free_floor_node_advanced(*out_floor);
		*out_floor = NULL;
		return (0);
	}
	return (handle_floor_player_advanced(global,
			out_floor, index));
}

static void	connect_neighbor_floors_advanced(t_floor *current,
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

static int	attach_existing_neighbor_advanced(t_floor *current,
		t_floor *neighbor, int is_up, int index)
{
	neighbor->index = index;
	connect_neighbor_floors_advanced(current, neighbor, is_up);
	return (1);
}

static void	push_floor_node_advanced(t_cub_data *data, t_floor *neighbor)
{
	neighbor->next = data->floors;
	data->floors = neighbor;
	data->floor_count++;
}

static int	parse_neighbor_floor_advanced(t_floor *current,
		const char *neighbor_path, int is_up, t_cub_data *data)
{
	t_floor	*neighbor;
	int		index;
	char	*normalized;

	if (!neighbor_path || *neighbor_path == '\0')
		return (1);
	normalized = normalize_path_advanced(neighbor_path);
	if (!normalized)
		return (0);
	neighbor = find_floor_by_path_advanced(data->floors, normalized);
	if (is_up)
		index = current->index + 1;
	else
		index = current->index - 1;
	free(normalized);
	if (neighbor)
		return (attach_existing_neighbor_advanced(current, neighbor, is_up,
				index));
	if (!parse_floor_file_advanced(neighbor_path, index, data, &neighbor))
		return (0);
	push_floor_node_advanced(data, neighbor);
	connect_neighbor_floors_advanced(current, neighbor, is_up);
	return (1);
}

static int	parse_floor_neighbors_advanced(t_floor *floor, t_cub_data *data)
{
	if (!floor || floor->parsed_neighbors)
		return (1);
	floor->parsed_neighbors = 1;
	if (!parse_neighbor_floor_advanced(floor, floor->up_path, 1, data))
		return (0);
	if (!parse_neighbor_floor_advanced(floor, floor->down_path, 0, data))
		return (0);
	if (floor->up && !parse_floor_neighbors_advanced(floor->up, data))
		return (0);
	if (floor->down
		&& !parse_floor_neighbors_advanced(floor->down, data))
		return (0);
	return (1);
}

static int	validate_global_elevators_advanced(t_cub_data *data)
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

static int	setup_root_floor_advanced(const char *path, t_cub_data *data,
		t_floor **root)
{
	char	*canonical_root;

	canonical_root = normalize_path_advanced(path);
	if (!canonical_root)
		return (0);
	if (!create_floor_from_data_advanced(canonical_root, data, root, 0))
		return (0);
	data->floors = *root;
	data->current_floor = *root;
	data->map = (*root)->map;
	data->floor_count = 1;
	if (!register_floor_elevators_advanced(data, *root))
		return (0);
	if ((*root)->has_player)
		assign_global_player_floor_advanced(data, *root, 0, path);
	return (1);
}

static int	validate_player_presence_advanced(t_cub_data *data)
{
	if (!data->player_floor_path)
	{
		printf("Error: Expected exactly one player across all floors\n");
		return (0);
	}
	return (1);
}

static void	sync_current_floor_visuals_advanced(t_cub_data *data)
{
	data->textures = data->current_floor->textures;
	data->floor_color = data->current_floor->floor_color;
	data->ceiling_color = data->current_floor->ceiling_color;
}

int	build_floor_graph_advanced(const char *path, t_cub_data *data)
{
	t_floor	*root;

	if (!setup_root_floor_advanced(path, data, &root))
		return (0);
	if (!parse_floor_neighbors_advanced(root, data))
		return (0);
	if (!validate_player_presence_advanced(data))
		return (0);
	if (!validate_global_elevators_advanced(data))
		return (0);
	sync_current_floor_visuals_advanced(data);
	return (1);
}
