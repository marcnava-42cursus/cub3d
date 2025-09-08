/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 02:25:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/09/02 02:25:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"

static void	init_cub_data(t_cub_data *data)
{
	data->textures.north = NULL;
	data->textures.south = NULL;
	data->textures.west = NULL;
	data->textures.east = NULL;
	data->floor_color.r = -1;
	data->floor_color.g = -1;
	data->floor_color.b = -1;
	data->ceiling_color.r = -1;
	data->ceiling_color.g = -1;
	data->ceiling_color.b = -1;
	data->map.grid = NULL;
	data->map.width = 0;
	data->map.height = 0;
	data->player.x = -1.0f;
	data->player.y = -1.0f;
	data->player.angle = 0.0f;
	data->player.orientation = 0;
}

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

static int	find_map_start(char **lines, int line_count)
{
	int	i;
	int	found_all_elements;
	int	texture_count;
	int	color_count;

	found_all_elements = 0;
	texture_count = 0;
	color_count = 0;
	i = 0;
	while (i < line_count)
	{
		if (is_empty_line(lines[i]))
		{
			i++;
			continue;
		}
		if (is_texture_identifier(lines[i]))
			texture_count++;
		else if (is_color_identifier(lines[i]))
			color_count++;
		else if (is_map_line(lines[i]) && texture_count == 4 && color_count == 2)
		{
			found_all_elements = 1;
			break;
		}
		else if (!is_texture_identifier(lines[i])
			&& !is_color_identifier(lines[i])
			&& !is_empty_line(lines[i]))
		{
			if (texture_count < 4 || color_count < 2)
				return (-1);
			break;
		}
		i++;
	}
	if (!found_all_elements)
		return (-1);
	return (i);
}

static int	validate_parsed_data(const t_cub_data *data)
{
	if (!data->textures.north || !data->textures.south
		|| !data->textures.west || !data->textures.east)
		return (0);
	if (data->floor_color.r == -1 || data->ceiling_color.r == -1)
		return (0);
	if (!data->map.grid || data->map.height == 0 || data->map.width == 0)
		return (0);
	if (data->player.x == -1.0f || data->player.y == -1.0f)
		return (0);
	return (1);
}

static void	free_lines(char **lines, int line_count)
{
	int	i;

	i = 0;
	while (i < line_count)
	{
		free(lines[i]);
		i++;
	}
	free(lines);
}

static int	process_file_data(char **lines, int line_count, t_cub_data *data)
{
	int	map_start;

	map_start = find_map_start(lines, line_count);
	if (map_start == -1)
		return (0);
	if (!parse_elements(lines, map_start, data))
		return (0);
	if (!parse_map_section(lines, map_start, data))
		return (0);
	if (!validate_parsed_data(data))
		return (0);
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
