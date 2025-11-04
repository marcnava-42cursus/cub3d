/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_orchestrator.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 00:00:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/11/04 20:39:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	init_cub_data(t_cub_data *data);

static int	parse_elements(char **lines, int map_start, t_cub_data *data)
{
	int	i;

	i = 0;
	while (i < map_start)
	{
		if (is_empty_line(lines[i]))
		{
			i++;
			continue ;
		}
		if (is_texture_identifier(lines[i]))
		{
			if (!parse_texture_line(lines[i], &data->textures))
				return (0);
		}
		else if (is_color_identifier(lines[i]))
		{
			if (!parse_color_line(lines[i], &data->floor_color,
					&data->ceiling_color))
				return (0);
		}
		i++;
	}
	return (1);
}

static int	validate_parsed_data(const t_cub_data *data)
{
	if (!data->textures.north_path || !data->textures.south_path
		|| !data->textures.west_path || !data->textures.east_path)
		return (0);
	if (data->floor_color.r == -1 || data->ceiling_color.r == -1)
		return (0);
	if (!data->map.grid || data->map.height == 0 || data->map.width == 0)
		return (0);
	if (data->player.x == -1.0f || data->player.y == -1.0f)
		return (0);
	return (1);
}

static int	process_file_data(char **lines, int line_count, t_cub_data *data)
{
	int	map_start;

	map_start = find_map_start(lines, line_count);
	if (map_start == -1)
	{
		printf("Error: Map section not found\n");
		return (0);
	}
	if (!parse_elements(lines, map_start, data))
	{
		printf("Error: Failed parsing elements (textures/colors) before map\n");
		return (0);
	}
	if (!parse_map_section(lines, map_start, data))
	{
		printf("Error: Failed parsing map section\n");
		return (0);
	}
	if (!validate_parsed_data(data))
	{
		printf(
			"%s Parsed data invalid (missing textures/colors/map/player)\n",
			"Error: ");
		return (0);
	}
	return (1);
}

int	parse_cub_file(const char *filename, t_cub_data *data)
{
	char	**lines;
	int		line_count;

	if (!validate_file_extension(filename))
	{
		printf("Error: Invalid file extension. Expected .cub\n");
		return (0);
	}
	init_cub_data(data);
	lines = read_file_lines(filename, &line_count);
	if (!lines)
	{
		printf("Error: Could not read file %s\n", filename);
		return (0);
	}
	if (!process_file_data(lines, line_count, data))
	{
		free_cub_data(data);
		free_lines(lines, line_count);
		return (0);
	}
	free_lines(lines, line_count);
	return (1);
}
