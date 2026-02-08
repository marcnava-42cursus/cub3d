/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_parse_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 03:45:00 by marcnava          #+#    #+#             */
/*   Updated: 2026/02/08 03:30:53 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	create_floor_from_data_bonus(char *canonical_path,
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
	init_floor_node_fields_bonus(*out, canonical_path, src, index);
	reset_source_floor_data_bonus(src);
	if (!collect_floor_elevators_bonus(*out))
	{
		free_floor_node_bonus(*out);
		*out = NULL;
		return (0);
	}
	return (1);
}

int	prepare_tmp_floor_parse_bonus(const char *path, char *canonical,
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
	if (!process_file_data_bonus(lines, line_count, tmp))
		return (free_lines(lines, line_count), free_cub_data(tmp), 0);
	free_lines(lines, line_count);
	return (1);
}

void	assign_global_player_floor_bonus(t_cub_data *global,
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

int	handle_floor_player_bonus(t_cub_data *global,
		t_floor **out_floor, int index)
{
	if (!(*out_floor)->has_player)
		return (1);
	if (global->player_floor_path)
	{
		printf("Error: Multiple player positions across floors\n");
		printf("  - First player found in: %s\n", global->player_floor_path);
		printf("  - Another player found in: %s\n", (*out_floor)->path);
		free_floor_node_bonus(*out_floor);
		*out_floor = NULL;
		return (0);
	}
	assign_global_player_floor_bonus(global, *out_floor, index,
		(*out_floor)->path);
	return (1);
}

int	parse_floor_file_bonus(const char *path, int index,
		t_cub_data *global, t_floor **out_floor)
{
	t_cub_data	tmp;
	char		*canonical;

	if (!validate_file_extension(path))
	{
		printf("Error: Invalid floor file extension for %s\n", path);
		return (0);
	}
	canonical = normalize_path_bonus(path);
	if (!canonical)
		return (0);
	if (!prepare_tmp_floor_parse_bonus(path, canonical, &tmp))
		return (free(canonical), 0);
	if (!create_floor_from_data_bonus(canonical, &tmp, out_floor, index))
		return (free_cub_data(&tmp), 0);
	free_cub_data(&tmp);
	if (!register_floor_elevators_bonus(global, *out_floor))
	{
		free_floor_node_bonus(*out_floor);
		*out_floor = NULL;
		return (0);
	}
	return (handle_floor_player_bonus(global, out_floor, index));
}
